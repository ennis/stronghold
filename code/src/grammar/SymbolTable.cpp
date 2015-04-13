#include <grammar/SymbolTable.hpp>
#include <iostream>
#include <stdexcept>

CSymbol *SymbolTable::addSymbol(const char *nt, SymbolType symType)
{
	std::pair<std::map<std::string, CSymbol*>::iterator, bool> ins =
		symbolTable.insert(std::pair<std::string, CSymbol*>(std::string(nt), NULL));

	if (ins.second) {
		// just created
		Symbol *sym = new Symbol;
		sym->name = std::string(nt);
		sym->type = symType;
		ins.first->second = sym;
		return sym;
	}
	else {
		return ins.first->second;
	}
}

CSymbol *SymbolTable::getSymbol(const char *sym) const
{
	std::map<std::string, CSymbol*>::const_iterator it = symbolTable.find(sym);
	if (it == symbolTable.end()) {
		throw std::runtime_error("Symbol not found");
	}

	return it->second;
}

void SymbolTable::printSymbols() const
{
	std::map<std::string, CSymbol*>::const_iterator it;
	for (it = symbolTable.begin(); it != symbolTable.end(); it++) {
		std::cout << it->first << ' ' << (it->second->type == ST_Terminal ? "Terminal" : "Non terminal") << std::endl;
	}
}
