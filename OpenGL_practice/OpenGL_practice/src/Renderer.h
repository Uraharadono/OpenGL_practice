#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
//ovdje pravim macro koji ce automatski zovnuti clear error funkciju itd. svaki put kad se dogodi error
//znaci u ovom slucaju pravim macro koji se zove "GLCall" koji prima x (x je funkcija)
//poziva se prvo "GLClearError", i onda ide backslash "\" da bi mogao nastaviti pisati ispod macro, jer bi bilo previse sve u liniji napisati.
//zatim ide x (to je nasa funckija koju smo proslijedili
//kad pozivamo funkciju (tj. x) trebamo proslijediti 3 paramentra
//kad uradimo "#x" to znaci da cemo uraditi x.ToString(), tkao cemo dobiti ime funkcije
//VELIKI NOTE: Cherno kaze da on SVAKU openGl funkciju wrapa u ovaj call, tako da bi trebao to zapamtiti da je best practice to (ne radi meni - makar na poslu)
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);