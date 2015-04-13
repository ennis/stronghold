#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const int WIDTH = 800;
static const int HEIGHT = 600;

#include "Shader.hpp"
#include "Program.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "SkyCube.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Random.hpp"
#include "Axes.hpp"
#include "Renderable.hpp"
#include "Grammar.hpp"
#include "FileWatcher.hpp"
#include "CylinderRenderable.hpp"
#include "Path.hpp"

#include "FieldFunction.hpp"
#include "GradientFunction.hpp"

// Antlr
#include "GrammarLexer.hpp"
#include "GrammarParser.hpp"

static void parseRuleFile(const char *path, Grammar &grammar)
{
	grammar.clear();
	GrammarLexerTraits::InputStreamType* input =
		new GrammarLexerTraits::InputStreamType((ANTLR_UINT8*)path, ANTLR_ENC_8BIT);
	GrammarLexer lexer(input);
	GrammarLexerTraits::TokenStreamType* tokenStream =
		new GrammarLexerTraits::TokenStreamType(ANTLR_SIZE_HINT, lexer.get_tokSource());
	GrammarParser parser(tokenStream);
	parser.g = &grammar;
	parser.regles();
}

static void clearContext(Context &ctx)
{
	std::clog << "Deleting context\n";
	for (std::list<Renderable*>::iterator it = ctx.renderables.begin();
		it != ctx.renderables.end(); ++it) {
		delete (*it);
	}
	ctx.renderables.clear();
	ctx.renderBatch.clear();
}

int main(int argc, char * argv[]) {
    double current_time = 0;
    double last_time = 0;
	double last_check = 0;

    GLFWwindow *window = createMainWindow(WIDTH, HEIGHT, "Strongholds");
    Camera::Instance().init(70.f, float(WIDTH) / float(HEIGHT), 0.01f, 5000.f);

	// watcher pour le fichier de règles
    FileWatcher watch;
    if (argc > 1) {
        watch.path = argv[1];
    }

	// grammaire chargée
    Grammar g;
	// contexte d'exécution
	Context ctx;
	
	// initialisation renderables
	Image heightmap;
	heightmap.load("textures/test_heightmap.png");
	Terrain terrain;
	terrain.init(heightmap);
	ctx.terrain = &terrain;
	Axes axes;
	axes.init();
	SkyCube sky;
	sky.init();

    RenderData rd;
    // center camera on the terrain
    glm::vec2 ts = terrain.getSize();
    Camera::Instance().transform.move(glm::vec3(ts.x / 2, 50.f, ts.y / 2));

	// initialisation render data
	rd.sunDirection = glm::normalize(glm::vec3(0.2f, -1.f, 0.2f));
	rd.sunColor = glm::vec3(1.f, 1.f, 0.7f);
	rd.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	rd.enableAmbientOcclusion = false;

	bool file_loaded = false;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        last_time = current_time;
        current_time = glfwGetTime();
        double dt = current_time - last_time;
		if (argc > 1) {
            last_check += dt;
			if (last_check > 1.f || !file_loaded) {
                // vérification du changement du fichier à chaque seconde
                last_check = 0.f;
				if (watch.fileChanged() || !file_loaded) {
					GRandom.reset();
					file_loaded = true;
					clearContext(ctx);
                    parseRuleFile(argv[1], g);
					g.generate("S", ctx);
                }
            }
        }

		Camera &cam = Camera::Instance();
		cam.update(dt);
		rd.projMatrix = cam.projectionMatrix();
		rd.viewMatrix = cam.viewMatrix();
		//Ligne en dessous à commenter pour avoir un soleil fixe
		//rd.sunDirection = glm::angleAxis(float(dt * 2 * M_PI * 0.01f), glm::vec3(0, 0, 1)) * rd.sunDirection;
		rd.cameraPos = cam.transform.position;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		rd.viewportSize = glm::vec2(width, height);

        glClearColor(226.f / 255.f, 236.f / 255.f, 124.f / 255.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			rd.enableAmbientOcclusion = !rd.enableAmbientOcclusion;
		}

		Camera::Instance().update(dt);
		rd.projMatrix = Camera::Instance().projectionMatrix();
		rd.viewMatrix = Camera::Instance().viewMatrix();

		glClearColor(226.f / 255.f, 236.f / 255.f, 124.f / 255.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		axes.render(rd);
		terrain.render(rd);
		sky.render(rd);
		ctx.renderBatch.render(rd);

		// Rendu des objets générés
		for (std::list<Renderable*>::iterator it = ctx.renderables.begin();
				it != ctx.renderables.end(); ++it)
		{
			(*it)->render(rd);
		}

        glfwSwapBuffers(window);
    }

    // export generated geometry
    std::ofstream outputGeom("output.obj");
    ctx.renderBatch.exportToOBJ(outputGeom);
 	outputGeom.close();

    // Memory freeing..
    glfwDestroyWindow(window);
    glfwTerminate();
	// TODO : vider les liste de fonctions *, car la std ne les supprimera pas correctement

    return 0;
}
