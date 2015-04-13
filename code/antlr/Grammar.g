grammar Grammar;

options
{
    language=Cpp;
}

tokens
{
    COLON = ':';
    SEMI = ';';
    LPAREN = '(';
    RPAREN = ')';
    ARROW = '->';
    COMMA = ',';
    LCURLYBRACE = '{';
    RCURLYBRACE = '}';
    EQUAL = '=';
    NOT_EQUAL = '!=';
    DDOTS = '..';
}

@lexer::traits {
    class GrammarLexer;
    class GrammarParser;
    typedef antlr3::Traits<GrammarLexer, GrammarParser> GrammarLexerTraits;
    typedef GrammarLexerTraits GrammarParserTraits;
}

@header {
#include <stdio.h>
#include <iostream>
#include "GrammarLexer.hpp"
#include <glm/glm.hpp>
#include <sstream>
#include "Rule.hpp"
#include "Grammar.hpp"

#include "Rules.hpp"
#include "Constraints.hpp"
#include "SplitEnums.hpp"
}

@context {
    Grammar *g;

    int getInt(std::string const &text) {
        int i;
        std::stringstream ss;
        ss.str(text);
        ss >> i;
        return i;
    }

    float getFloat(std::string const &text) {
        float f;
        std::stringstream ss;
        ss.str(text);
        ss >> f;
        return f;
    }
}


/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

regles: regle+;

regle
@init {
	int index;
    ListConstraints *lc = new ListConstraints ();
}: id=NUMBER { index = getInt($id.text); } COLON  p=symbol ARROW r=regle_rhs[index, $p.sym] (COLON LCURLYBRACE conds[lc] {$r.rule->setListConstraints(lc);} RCURLYBRACE)? (COLON prob=NUMBER {$r.rule->setWeight(getFloat($prob.text));} )? SEMI { g->addRule(index, $r.rule); };

regle_rhs[int index, CSymbol *pred] returns [Rule* rule]
@init{
	$rule = NULL;
}:
(op=operation[$index, $pred] {$rule = $op.rule;})?
(sym=symbol {
	if ($rule == NULL) {
		$rule = new DummyRule($index, $pred);
	}

    if ($sym.sym->name != "epsilon") {
        $rule->addSuccessor($sym.sym);
    }
}
)* ;

// CONSTRAINTS
conds [ListConstraints *lc] : c1=cond {
    lc->addConstraint($c1.c);
}
(COMMA c2=cond {
    lc->addConstraint($c2.c);
})*
;

cond returns [Constraint *c] : UNIQUE {
    $c = new ConstraintUniqueness();
}
| EVEN {
    $c = new ConstraintEven();
}
| ODD {
    $c = new ConstraintOdd();
}
| INDEX EQUAL i=NUMBER {
    $c = new ConstraintIndex(getInt($i.text), false);
}
| INDEX NOT_EQUAL i=NUMBER {
    $c = new ConstraintIndex(getInt($i.text), true);
}
| FLAT {
    $c = new ConstraintFlat();
}
| TOWER {
    $c = new ConstraintTower();
}
;

operation[int index, CSymbol *pred] returns [Rule *rule] :
	  poly=opPoly2D[$index, $pred] { $rule = $poly.poly; }
	| show=opShow[$index, $pred] { $rule = $show.rule; }
	| citywalls=opCityWalls[$index, $pred] { $rule = $citywalls.rule; }
    | build_cylinder=opCylinder[$index, $pred] {$rule = $build_cylinder.rule; }
    | build_rectangle=opRectangle[$index, $pred] {$rule = $build_rectangle.rule; }
    | build_houses=opHouses[$index, $pred] { $rule = $build_houses.rule; }
    | select_faces=opSelectfaces[$index, $pred] { $rule = $select_faces.rule; }
    | extrude=opExtrude[$index, $pred] { $rule = $extrude.rule; }
	| model=opModel[$index, $pred] { $rule = $model.rule; }
    | door=opDoor[$index, $pred] {$rule = $door.rule; }
    | roof=opRoof[$index, $pred] {$rule = $roof.rule; }
    | splitEqually=opSplitEqually[$index, $pred] {$rule = $splitEqually.rule; }
	| splitProportions=opSplitProportions[$index, $pred] {$rule = $splitProportions.rule; }
	| splitRepeat=opSplitRepeat[$index, $pred] {$rule = $splitRepeat.rule; }
    | strongholdSeed=opStrongholdSeed[$index, $pred] {$rule = $strongholdSeed.rule; }
    | village=opMakeVillage[$index, $pred] { $rule = $village.rule; }
	| translation=opTranslation[$index, $pred] { $rule = $translation.rule; }
    | dungeon=opDungeon[$index, $pred] { $rule = $dungeon.rule; }
    | splitCylinder=opSplitCylinder[$index, $pred] { $rule = $splitCylinder.rule; }
    | arrowSlit=opArrowSlit[$index, $pred] { $rule = $arrowSlit.rule; }
    | edges=opSelectEdges[$index, $pred] { $rule = $edges.rule; }
;

symbol returns [CSymbol *sym]: STRING {
    // Si le symbole est déjà présent dans la table des symboles, alors c'est un terminal
	// Sinon, c'est un non terminal défini par l'utilisateur
	$sym = g->addSymbol($text.c_str(), ST_NonTerminal);
};

//---------------------------------------
// Polygon2DRule
//
//---------------------------------------
opPoly2D[int index, CSymbol *pred] returns [Polygon2DRule *poly]
@init{
    $poly = new Polygon2DRule($index, $pred);
}: POLYGON2D LPAREN polygon2DArgs[$poly] RPAREN {
};

polygon2DArgs[Polygon2DRule *poly]
 : c=coordArg2D {
    $poly->addPoint($c.coord);
} (COMMA d=coordArg2D {
    $poly->addPoint($d.coord);
})*
;

coordArg2D returns [glm::vec2 coord]: LPAREN x=NUMBER COMMA y=NUMBER RPAREN {
    $coord = glm::vec2(getFloat($x.text), getFloat($y.text));
};

coordArg3D returns [glm::vec3 coord]: LPAREN x=NUMBER COMMA y=NUMBER COMMA z=NUMBER RPAREN {
    $coord = glm::vec3(getFloat($x.text), getFloat($y.text), getFloat($z.text));
};

//---------------------------------------
// ShowRule
//
//---------------------------------------

opShow[int index, CSymbol *pred] returns [ShowRule *rule]
@init {
    $rule = new ShowRule($index, $pred);
} : SHOW (LPAREN tex=string_literal { $rule->setTexture($tex.literal); } RPAREN )?;

//---------------------------------------
// CityWalls
//
//---------------------------------------
opCityWalls[int index, CSymbol *pred] returns [CityWalls *rule] : CITYWALLS params=opCityWallsParams
{
	$rule = new CityWalls($index, $pred, $params.width, $params.height);
};

opCityWallsParams returns [float width, float height] : LPAREN val=NUMBER COMMA val2=NUMBER RPAREN {
	$width = getFloat($val.text);
	$height = getFloat($val2.text);
};

//---------------------------------------
// select_faces
//
//---------------------------------------

opSelectfaces[int index, CSymbol *pred] returns [SelectFacesRule *rule] : SELECT_FACES
{
	$rule = new SelectFacesRule($index, $pred);
};

//---------------------------------------
// extrude
//
//---------------------------------------

opExtrude[int index, CSymbol *pred] returns [ExtrudeRule *rule] : EXTRUDE LPAREN height=NUMBER RPAREN
{
    $rule = new ExtrudeRule($index, $pred, getFloat($height.text));
};

//---------------------------------------
// build_cylinder
//---------------------------------------

opCylinder [int index, CSymbol *pred] returns [CylinderRule *rule] : BUILD_CYLINDER LPAREN rayon_min=NUMBER DDOTS rayon_max=NUMBER COMMA hauteur_min=NUMBER DDOTS hauteur_max=NUMBER RPAREN
{
    $rule = new CylinderRule($index, $pred, getFloat($rayon_min.text), getFloat($rayon_max.text), getFloat($hauteur_min.text), getFloat($hauteur_max.text));
};

//---------------------------------------
// build_rectangle
//---------------------------------------

opRectangle [int index, CSymbol *pred] returns [RectangleRule *rule] : BUILD_RECTANGLE LPAREN width=NUMBER COMMA height=NUMBER COMMA length=NUMBER RPAREN
{
    $rule = new RectangleRule($index, $pred, getFloat($width.text), getFloat($height.text), getFloat($length.text));
};

//---------------------------------------
// model(scale, filename, texture)
//---------------------------------------

opModel [int index, CSymbol *pred] returns [ModelRule *rule] :
 MODEL LPAREN scale=NUMBER COMMA filename=string_literal COMMA texname=string_literal RPAREN
{
    $rule = new ModelRule($index, $pred, getFloat($scale.text), $filename.literal.c_str(), $texname.literal.c_str());
}
;

//---------------------------------------
// build_houses
//---------------------------------------

opHouses [int index, CSymbol *pred] returns [HousesRule *rule]
@init {
    $rule = new HousesRule($index, $pred);
}: BUILD_HOUSES LPAREN num=NUMBER COMMA params=opHousesParam[$rule] RPAREN {
    $rule->setNum(getInt($num.text));
};

opHousesParam[HousesRule* rule]
 : c=coordArg3D {
    $rule->addDimension($c.coord);
} (COMMA d=coordArg3D {
    $rule->addDimension($d.coord);
})*
;

string_literal returns [std::string literal] : str=STRING_LITERAL
{
	$literal = $str.text;
	$literal.erase(0, 1);
	$literal.erase($literal.size()-1, 1);
};

//---------------------------------------
// build_roof
//---------------------------------------
opRoof [int index, CSymbol *pred] returns [RoofRule *rule] : BUILD_ROOF LPAREN height=NUMBER RPAREN
{
    $rule = new RoofRule($index, $pred, getFloat($height.text));
};

//---------------------------------------
// build_door
//---------------------------------------
opDoor [int index, CSymbol *pred] returns [BuildDoorRule *rule] : BUILD_DOOR
{
    $rule = new BuildDoorRule($index, $pred);
};

//---------------------------------------
// Splitting
//---------------------------------------

// splitEqually(direction, numDivisions)
opSplitEqually [int index, CSymbol *pred] returns [SplitEquallyRule *rule] : SPLITEQUALLY LPAREN splitDirection COMMA numDivisions=NUMBER RPAREN
{
    $rule = new SplitEquallyRule($index, $pred, $splitDirection.dir, getInt($numDivisions.text));
}
;

// splitProportions(direction, (x1,x1,...))
opSplitProportions [int index, CSymbol *pred] returns [SplitProportionsRule *rule] :
	SPLITPROPORTIONS LPAREN dir=splitDirection {$rule = new SplitProportionsRule($index, $pred, $dir.dir);}
	COMMA LPAREN splitProportionsArgs[$rule] RPAREN RPAREN
{
}
;

splitProportionsArgs[SplitProportionsRule *rule]
 : c=NUMBER {
	$rule->addProportions(getFloat($c.text));
} (COMMA d=NUMBER {
	$rule->addProportions(getFloat($d.text));
})*
;

// splitRepeat(direction, (l1,l2,...), nb_repeat, alignment)
opSplitRepeat [int index, CSymbol *pred] returns [SplitRepeatRule *rule] :
	SPLITREPEAT LPAREN splitDirection {$rule = new SplitRepeatRule($index, $pred, $splitDirection.dir);}
	COMMA LPAREN splitRepeatArgs[$rule] RPAREN
	COMMA numRepeat=NUMBER {
		$rule->setNbRepeat(getInt($numRepeat.text));
	}
	COMMA splitAlignment {
		$rule->setAlignment($splitAlignment.alignment);
}
	RPAREN
;

splitRepeatArgs[SplitRepeatRule *rule]
 : c=NUMBER {
	$rule->addLength(getFloat($c.text));
} (COMMA d=NUMBER {
	$rule->addLength(getFloat($d.text));
})*
;


splitDirection returns [SplitDirection dir] :
	SPLIT_X { $dir = Split_X; }
	| SPLIT_Y { $dir = Split_Y; }
	| SPLIT_Z { $dir = Split_Z; }
;

splitAlignment returns [SplitAlignment alignment] :
	SPLIT_BEGIN { $alignment = Split_Begin; }
	| SPLIT_MIDDLE { $alignment = Split_Middle; }
	| SPLIT_END { $alignment = Split_End; }
;

//--------------------------------------
// stronghold_seed
//---------------------------------------

opStrongholdSeed [int index, CSymbol *pred] returns [StrongholdSeedRule *rule] : STRONGHOLD_SEED LPAREN c=coordArg2D COMMA faces=NUMBER COMMA radius=NUMBER RPAREN
{
    $rule = new StrongholdSeedRule($index, $pred, $c.coord, getInt($faces.text), getFloat($radius.text));
};

//--------------------------------------
// make_village
//---------------------------------------

opMakeVillage [int index, CSymbol *pred] returns [MakeVillageRule *rule] : MAKE_VILLAGE
{
    $rule = new MakeVillageRule($index, $pred);
};

//-------------------------
// translation 
//-------------------------

opTranslation[int index, CSymbol *pred] returns [TranslationRule *rule] : TRANSLATION LPAREN x=NUMBER COMMA y=NUMBER RPAREN
{
    $rule = new TranslationRule($index, $pred, getFloat($x.text), getFloat($y.text));
};

//--------------------------------------
// build_dungeon
//---------------------------------------
opDungeon [int index, CSymbol *pred] returns [BuildDungeonRule *rule] : BUILD_DUNGEON LPAREN radius=NUMBER COMMA height=NUMBER RPAREN
{
    $rule = new BuildDungeonRule($index, $pred, getFloat($radius.text), getFloat($height.text));
};

//--------------------------------------
// split_cylinder
//---------------------------------------
opSplitCylinder [int index, CSymbol *pred] returns [SplitCylinderRule *rule] : SPLIT_CYLINDER LPAREN divisions=NUMBER RPAREN
{
    $rule = new SplitCylinderRule($index, $pred, getInt($divisions.text));
};

//--------------------------------------
// make_arrow_slit
//---------------------------------------
opArrowSlit [int index, CSymbol *pred] returns [ArrowSlitRule *rule] : ARROW_SLIT
{
    $rule = new ArrowSlitRule($index, $pred);
};

//--------------------------------------
// select_edges
//---------------------------------------
opSelectEdges [int index, CSymbol *pred] returns [SelectEdgesRule *rule] : SELECT_EDGES LPAREN w=NUMBER RPAREN
{
    $rule = new SelectEdgesRule($index, $pred, getFloat($w.text));
};

/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

CUBE : 'cube';

// RULES
POLYGON2D : 'Polygon2D';
SHOW : 'show';
CITYWALLS : 'citywalls';
BUILD_CYLINDER : 'build_cylinder';
BUILD_RECTANGLE : 'build_rectangle';
BUILD_HOUSES : 'build_houses';
BUILD_ROOF : 'build_roof';
BUILD_DOOR: 'build_door';
SELECT_FACES: 'select_faces';
EXTRUDE : 'extrude';
SPLITEQUALLY : 'splitEqually';
SPLITPROPORTIONS : 'splitProportions';
SPLITREPEAT : 'splitRepeat';
SPLIT_X : 'X';
SPLIT_Y : 'Y';
SPLIT_Z : 'Z';
SPLIT_BEGIN : 'Begin';
SPLIT_MIDDLE : 'Middle';
SPLIT_END : 'End';
SPLIT_CYLINDER : 'split_cylinder';
MODEL : 'model';
STRONGHOLD_SEED : 'stronghold_seed';
MAKE_VILLAGE : 'make_village';
TRANSLATION : 'translation';
BUILD_DUNGEON : 'build_dungeon';
ARROW_SLIT : 'make_arrow_slit';
SELECT_EDGES : 'select_edges';

// CONSTRAINTS
UNIQUE : 'unique';
EVEN : 'even';
ODD : 'odd';
INDEX : 'index';
FLAT : 'flat';
TOWER : 'tower';

// TOKENS

COMMENT : '//' ~('\n'|'\r')* '\r'? '\n'
{
	$channel = HIDDEN;
};

NUMBER :   SIGN (DIGIT)+ '.' (DIGIT)*
    | SIGN (DIGIT)+;

STRING      : (LETTER) (DIGIT | LETTER)*;

WHITESPACE  : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+
{
    $channel = HIDDEN;
};

//chaines et caracteres
// fragment : on attend d'autres caracteres pour construire la string
// tous ce qui n'est pas un caractere critique \\ represente un \ 'seul' en antlr
fragment STRING_CAR : ~('\\' | '"' | '\n');
// ajout des caracteres echappes sauf EOL \\" pour \" et \\\\ pour \\
STRING_LITERAL : '"' (STRING_CAR | '\\\\' | '\\"')* '"';

fragment LETTER   : 'a' .. 'z' | 'A' .. 'Z';
fragment DIGIT    : '0'..'9';
fragment INTEGER  : (DIGIT)+;
fragment DEC      : INTEGER '.' INTEGER;
fragment SIGN     : '+' | '-' | /* epsilon */;
fragment EXP      : ('E' | 'e') SIGN INTEGER;
