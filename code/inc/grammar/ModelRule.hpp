#ifndef MODELRULE_HPP
#define MODELRULE_HPP

#include <Rule.hpp>
#include <Model.hpp>
#include <Texture.hpp>

class ModelRule : public Rule
{
public:
	ModelRule(int i, Symbol const *predecessor, float scale, const char *modelName, const char *textureName);

	void execute(Grammar &grammar, Context &context, Shape &shape);

private:
	std::string modelName;
	std::string textureName;
	float scale;
	Model *model;
	Texture *texture;
};

#endif