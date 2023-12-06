/*
 * projeto.cpp
 * gcc projeto.cpp -o projeto -lSOIL -lglut -lGL -lGLU
 * ./plojeto
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SOIL/SOIL.h>

/**********************************************************************/
/*                      Declaração de tipos especiais                 */
/**********************************************************************/

typedef struct Atom
{
	char elemento[255];
	int protons;
	int neutrons;
	int alfa;
	int beta;
	int radius;
} Atom;

enum ElectronicLayers : GLint
{
	K,
	L,
	M,
	N,
	O,
	P,
	Q
};

/**********************************************************************/
/*                      Declaração de constantes                      */
/**********************************************************************/

#define INV -1

/*  Cores Prótons  */
const GLfloat proton[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat proton_ambient[] = {0.8, 0.0, 0.0, 1.0};
const GLfloat proton_emission[] = {0.1, 0.0, 0.0, 1.0};

/*  Cores Elétrons  */
const GLfloat eletron[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat eletron_ambient[] = {0.0, 0.0, 0.8, 1.0};
const GLfloat eletron_emission[] = {0.0, 0.0, 0.1, 1.0};

/*  Cores Neutrons  */
const GLfloat neutron[] = {0.5, 0.5, 0.5, 1.0};
const GLfloat neutron_ambient[] = {0.4, 0.4, 0.4, 1.0};
const GLfloat neutron_emission[] = {0.05, 0.05, 0.05, 1.0};

const GLfloat white_color[] = {1.0, 1.0, 1.0, 1.0}; // White emission

const GLfloat high_shininess[] = {100.0};

const GLfloat not_fermi_radius = 0.1;

const GLint frames_per_second = 10;

const GLfloat beta_button[2] = {5.25, 0.0};
const GLfloat alfa_button[2] = {-5.25, 0.0};
const GLfloat button_radius = 0.5;

/*const Atom decay238[] = {
	{"U - Uranium",  92, 238 - 92, 1,   INV},	// 0
	{"Th - Thorium", 90, 234 - 90, INV, 2},	// 1
	{"Pa - Protactinium", 91, 234 - 91, INV, 3},	// 2
	{"U - Uranium",  92, 234 - 92, 4,   INV},	// 3
	{"Th - Thorium", 90, 230 - 90, 5,   INV},	// 4
	{"Ra", 88, 226 - 88, 6,   INV},	// 5
	{"Rn", 86, 222 - 86, 7,   INV},	// 6
	{"Po", 84, 218 - 84, 8,   9},	// 7
	{"Pb", 82, 214 - 82, INV, 10},	// 8
	{"At", 84, 218 - 85, 10,  INV},	// 9
	{"Bi", 83, 214 - 83, 11,  12},	// 10
	{"Ti", 81, 210 - 81, INV, 13},	// 11
	{"Po", 84, 214 - 84, 13,  INV},	// 12
	{"Pb", 82, 210 - 82, 14,  15},	// 13
	{"Hg", 80, 206 - 80, INV, 16},	// 14
	{"Bi", 83, 210 - 83, 16,  17},	// 15
	{"Ti", 81, 206 - 81, INV, 18},	// 16
	{"Po", 84, 210 - 84, 18,  INV},	// 17
	{"Pb", 82, 206 - 82, INV, INV}, // 18
};*/

const Atom decay[] = {
	{"U - Uranium", 92, 235 - 92, 1, INV, 196},				 // 0 ok
	{"Th - Thorium", 90, 231 - 90, INV, 2, 206},			 // 1 ok
	{"Pa - Protactinium", 91, 231 - 91, 3, INV, 200},		 // 2 ok
	{"Ac - Actinium", 89, 227 - 89, 5, 4, 215},				 // 3 ok
	{"Th - Thorium", 90, 227 - 90, 6, INV, 206},			 // 4 ok
	{"Fr - Francium", 87, 223 - 87, 7, 6, 260},				 // 5 ok (sem imagem)
	{"Ra - Radium", 88, 223 - 88, 8, INV, 221},				 // 6 ok
	{"At - Astatine", 85, 219 - 85, 9, 8, 150},				 // 7 ok
	{"Rn - Radon 219 (Actinon)", 86, 219 - 86, 10, INV, 150}, // 8 ok (sem imagem)
	{"Bi - Bismuth", 83, 215 - 83, INV, 10, 148},			 // 9 ok
	{"Po - Polonium", 84, 215 - 84, 11, 12, 140},			 // 10
	{"Pb - Lead", 82, 211 - 82, INV, 13, 146},				 // 11
	{"At - Astatine", 85, 215 - 85, 13, INV, 150},			 // 12
	{"Bi - Bismuth", 83, 211 - 83, 14, 15, 148},			 // 13
	{"Ti - Thalium", 81, 207 - 81, INV, 16, 145},			 // 14
	{"Po - Polonium", 83, 210 - 83, 16, INV, 140},			 // 15
	{"Pb - Lead", 82, 207 - 82, INV, INV, 148},				 // 16
};

const char *text_Atom = "%s\nAtomic Mass: %d\nAtomic Number: %d\nAlfa: %s\nBeta: %s";

/**********************************************************************/
/*                      Declaração de variáveis                       */
/**********************************************************************/

static GLfloat relative_color[] = {0.0, 1.0, 0.0, 1.0};

static GLfloat angulo = 0.0;

static GLint window_width = 0.0;
static GLint window_height = 0.0;

static GLint atom_index = 0;

static GLboolean rotating = true;

GLint camadas[10];
GLint layer_counter = 0;

GLfloat current_nuclear_radius = 0;
// GLfloat current_valency_radius = 0;

GLuint texture;

/**********************************************************************/
/*                        Declaração de funções                       */
/**********************************************************************/

void init_glut(const char *window_name, int *argcp, char **argv);

void display_callback(void);
void reshape_callback(int w, int h);
void keyboard_callback(unsigned char key, int x, int y);
void keyboard_callback_special(int key, int x, int y);
void mouse_callback(int button, int state, int x, int y);
void timer_callback(int fps);
void menu_callback(int value);

void draw_object(void);
void draw_interface(void);

void spinDisplay(void);

void draw_particle(float radius);
void draw_eletron(void);
void draw_proton(void);
void draw_neutron(void);

void draw_circle(const GLfloat c[2], float r);

void alfa_decay(void);
void beta_decay(void);

bool closer_than(const GLfloat c_A[2], const GLfloat c_B[2], const GLfloat d);
void render_text(const char *text, int x, int y);
void loadTexture(void);
void setElectronicLayers(void);

/**********************************************************************/
/*                                                                    */
/*                       Função principal (main)                      */
/*                                                                    */
/**********************************************************************/

int main(int argc, char **argv)
{

	// int aux[100] = {};
	// int bux = 0;
	// for(int i = 0; i<sizeof(decay)/sizeof(decay[0]);i++)aux[decay[i].protons]++;
	// for(int i = 0; i<100; i++){
	// 	if(aux[i]>0) {
	// 		bux++;
	// 		printf("%d\n", i);
	// 	}
	// }
	// printf("%d\n", bux);

	/* inicia o GLUT e alguns parâmetros do OpenGL */
	init_glut("Nuclear Decay Chain Interactive Graphic System (NDCIGS)", &argc, argv);

	/* função de controlo do GLUT */
	glutMainLoop();

	return 0;
}

/**********************************************************************/
/*                                                                    */
/*                         Definicao de funcoes                       */
/*                                                                    */
/**********************************************************************/

/*
 * inicia o GLUT e alguns parametros do OpenGL
 */
void init_glut(const char *nome_janela, int *argcp, char **argv)
{
	/* inicia o GLUT */
	glutInit(argcp, argv);

	/* inicia o display usando RGB e double-buffering */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(nome_janela);

	/* define as funções de callback */
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutKeyboardFunc(keyboard_callback);
	glutSpecialFunc(keyboard_callback_special);
	glutMouseFunc(mouse_callback);
	glutTimerFunc(1000 / frames_per_second /*ms*/, timer_callback, frames_per_second);

	/* define o menu */
	glutCreateMenu(menu_callback);
	glutAddMenuEntry("Decaimento Alfa", 0);
	glutAddMenuEntry("Decaimento Beta", 1);
	glutAddMenuEntry("Uranio 235", 2);
	glutAddMenuEntry("Sair", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	GLfloat position[] = {0.0, 1.0, 1.0, 0.0};

	/* define a cor com a qual a tela será apagado */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	setElectronicLayers();
	loadTexture();

}

/**********************************************************************/
/*                                                                    */
/*                         Definicao de funcoes de callback           */
/*                                                                    */
/**********************************************************************/


/*
 * Exibe itens visuais
 */
void display_callback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_object();
	draw_interface();

	glutSwapBuffers();
}

/*
 * Processa o reshape da janela
 */
void reshape_callback(int w, int h)
{
	window_width = w;
	window_height = h;

	/* define a zona da janela onde se vai desenhar */
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	/* muda para o modo GL_PROJECTION e reinicia a projecao */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* adapta a projecao ortogonal de acordo com as dimensoes da janela */
	if (w <= h)
		glOrtho(-6.0, 6.0,
				-6.0 * ((GLfloat)h) / (GLfloat)w,
				6.0 * ((GLfloat)h) / (GLfloat)w,
				-10.0, 10.0);
	else
		glOrtho(-6.0 * (GLfloat)w / ((GLfloat)h),
				6.0 * (GLfloat)w / ((GLfloat)h),
				-6.0, 6.0, -10.0, 10.0);

	// printf("%d\t%d\n", w, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);

	/* muda para o modo GL_MODELVIEW (nao pretendemos alterar a projecao *
	 * quando estivermos a desenhar na tela)                             */
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

/*
 * Controle das teclas normais
 */
void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		alfa_decay();
		break;
	case 'b':
	case 'B':
		beta_decay();
		break;
	case 'u':
	case 'U':
		atom_index = 0;
		break;
	case 'r':
	case 'R':
		rotating = true;
		break;
	case 's':
	case 'S':
		rotating = false;
		break;
	case 27:
		exit(0); /* Esc: sai do programa */
		break;

	default:
		break;
	}

	// loadTexture();
	// setElectronicLayers();

}

/*
 * Controle das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: /* F1: muda a cor da interface para branco */
		relative_color[0] = 1.0;
		relative_color[1] = 1.0;
		relative_color[2] = 1.0;
		break;

	case GLUT_KEY_F2: /* F2: muda a cor da interface para magenta */
		relative_color[0] = 1.0;
		relative_color[1] = 0.0;
		relative_color[2] = 1.0;
		break;

	case GLUT_KEY_F3: /* F3: muda a cor da interface para azul */
		relative_color[0] = 0.0;
		relative_color[1] = 0.0;
		relative_color[2] = 1.0;
		break;

	case GLUT_KEY_F4: /* F4: muda a cor da interface para ciano */
		relative_color[0] = 0.0;
		relative_color[1] = 1.0;
		relative_color[2] = 1.0;
		break;

	case GLUT_KEY_F5: /* F5: muda a cor da interface para verde */
		relative_color[0] = 0.0;
		relative_color[1] = 1.0;
		relative_color[2] = 0.0;
		break;

	case GLUT_KEY_F6: /* F6: muda a cor da interface para amarelo */
		relative_color[0] = 1.0;
		relative_color[1] = 1.0;
		relative_color[2] = 0.0;
		break;

	case GLUT_KEY_F7: /* F7: muda a cor da interface para vermelho */
		relative_color[0] = 1.0;
		relative_color[1] = 0.0;
		relative_color[2] = 0.0;
		break;

	default:
		break;
	}

	return;
}

/*
 * Controle do mouse
 */
void mouse_callback(int button, int state, int x, int y)
{
	// Calcular coordenadas relativas a projecao da tela
	GLfloat relative_x = (GLfloat(x) / window_width) * 12.0 - 6.0;
	GLfloat relative_y = -(GLfloat(y) / window_height) * 12.0 + 6.0;

	if (window_width <= window_height)
	{
		GLfloat extended_height = 12.0 * ((GLfloat)window_height) / (GLfloat)window_width;
		relative_y = (GLfloat(y) / window_height) * extended_height - extended_height / 2.0;
	}
	else
	{
		GLfloat extended_width = 12.0 * (GLfloat)window_width / ((GLfloat)window_height);
		relative_x = (GLfloat(x) / window_width) * extended_width - extended_width / 2.0;
	}

	const GLfloat click[2] = {relative_x, relative_y};

	// printf("(%f,%f)\n", relative_x, relative_y);
	// fflush(stdout);

	// Lidar efetivamente com click
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (closer_than(alfa_button, click, button_radius))
				alfa_decay();
			else if (closer_than(beta_button, click, button_radius))
				beta_decay();
			else
				rotating = !rotating;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			rotating = false;
		break;
	default:
		break;
	}
}

/*
 * Controle de frames por segundo
 */
void timer_callback(int fps)
{
	spinDisplay();
	glutTimerFunc(1000 / fps, timer_callback, fps);
}

/*
 * Função callback para o controlo do menu
 */
void menu_callback(int value)
{
	// printf("Menu: %d\n", value);
	switch (value)
	{
	case 0:
		alfa_decay();
		break;

	case 1:
		beta_decay();
		break;

	case 2:
		atom_index = 0;
		break;

	case 10:
		exit(0); /* Opção 4: sai do programa */
		break;

	default:
		break;
	}


	// loadTexture();
	// setElectronicLayers();

	/* Manda o redesenhar o ecra quando o menu for desactivado */
	glutPostRedisplay();

	return;
}

/**********************************************************************/
/*                                                                    */
/*                         Definicao de funcoes de exibicao           */
/*                                                                    */
/**********************************************************************/


/*
 * Desenha atomo
 */
void draw_object(void)
{
	const GLint proton_number = decay[atom_index].protons;
	const GLint neutron_number = decay[atom_index].neutrons;
	const GLfloat raio_atomico = decay[atom_index].radius;

	GLint electron_number = 0;

	glPushMatrix();
	for (int i = 0; i < proton_number; i++)
	{
		glPushMatrix();

		glRotatef(360.0 * (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0);
		glTranslatef(current_nuclear_radius + 0.1 * (rand() % 100 - 50) / 50.0, 0, 0);

		// glRotatef((GLfloat)i * 360.0 / proton_number, 0.0, 1.0, 0.0);
		// glTranslatef(0.5 * (rand()%100-50)/50.0, 0.5 * (rand()%100-50)/50.0, 0.5 * (rand()%100-50)/50.0);
		draw_proton();
		glPopMatrix();
	}

	for (int i = 0; i < neutron_number; i++)
	{
		glPushMatrix();

		glRotatef(360.0 * (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0, (rand() % 100 - 50) / 50.0);
		glTranslatef(current_nuclear_radius + 0.1 * (rand() % 100 - 50) / 50.0, 0, 0);

		// glRotatef((GLfloat)(i * 360.0 / neutron_number + 180.0 / proton_number), 0.0, 1.0, 0.0);
		// glTranslatef(-0.5 * (rand()%100-50)/50.0, -0.5 * (rand()%100-50)/50.0, -0.5 * (rand()%100-50)/50.0);
		draw_neutron();
		glPopMatrix();
	}

	for (int i = 0; i < sizeof(camadas) / sizeof(camadas[0]); i++)
	{
		for (int j = 0; j < camadas[i]; j++)
		{
			glPushMatrix();
			glRotatef((GLfloat)angulo + j * 360.0 / camadas[i] + 2.5 * (rand() % 100 - 50) / 50.0, 0.0, 0.0, 1.0);
			glTranslatef(1.5 + 4.0 * ((raio_atomico / 200.0) / layer_counter) * i + 0.05 * (rand() % 100 - 50) / 50.0, 0.0, 0.0);
			draw_eletron();
			glPopMatrix();
			electron_number++;
			if (electron_number == proton_number)
				break;
		}
		if (electron_number == proton_number)
			break;
	}

	glPopMatrix();
}

/*
 * Desenha elementos da interface
 */
void draw_interface(void)
{
	/**********************************/
	/* Exibicao de texto na interface */
	/**********************************/

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// glRasterPos2i( 200, 100 );
	glRasterPos3i(10, 475, 1);

	char text[255];
	sprintf(text,
			text_Atom,
			decay[atom_index].elemento,
			decay[atom_index].protons + decay[atom_index].neutrons,
			decay[atom_index].protons,
			decay[atom_index].alfa > -1 ? decay[decay[atom_index].alfa].elemento : "-",
			decay[atom_index].beta > -1 ? decay[decay[atom_index].beta].elemento : "-");
	render_text(text, 10, 475 - 30);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	/**********************************/
	/* Exibicao de botoes na interface*/
	/**********************************/

	draw_circle(alfa_button, button_radius);

	draw_circle(beta_button, button_radius);

	/**********************************/
	/* Exibicao de imagem na interface*/
	/**********************************/

	glLoadIdentity();

	// Set material properties for emission
	glMaterialfv(GL_FRONT, GL_EMISSION, white_color);

	// Draw the plane with the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);	glVertex3f(-5.5, -5.5, 0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(-3.5, -5.5, 0.0);
		glTexCoord2f(1.0, 1.0);	glVertex3f(-3.5, -3.5, 0.0);
		glTexCoord2f(0.0, 1.0);	glVertex3f(-5.5, -3.5, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

/*
 * Atualiza angulo geral de rotacao e desenha atualiza display
 */
void spinDisplay(void)
{
	if (rotating)
	{
		angulo = angulo + 0.5;
		if (angulo > 360.0)
			angulo = angulo - 360.0;
	}
	glutPostRedisplay();
}

void draw_particle(float radius)
{
	glutSolidSphere(radius, 10, 8);
}

void draw_eletron(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, eletron_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, eletron);
	glMaterialfv(GL_FRONT, GL_EMISSION, eletron_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	draw_particle(0.06);
	glMaterialfv(GL_FRONT, GL_AMBIENT, relative_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, relative_color);
	glMaterialfv(GL_FRONT, GL_EMISSION, relative_color);
}

void draw_proton(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, proton_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, proton);
	glMaterialfv(GL_FRONT, GL_EMISSION, proton_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	draw_particle(0.15);
	glMaterialfv(GL_FRONT, GL_AMBIENT, relative_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, relative_color);
	glMaterialfv(GL_FRONT, GL_EMISSION, relative_color);
}

void draw_neutron(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, neutron_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, neutron);
	glMaterialfv(GL_FRONT, GL_EMISSION, neutron_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	draw_particle(0.15);
	glMaterialfv(GL_FRONT, GL_AMBIENT, relative_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, relative_color);
	glMaterialfv(GL_FRONT, GL_EMISSION, relative_color);
}

void draw_circle(const GLfloat c[2], float r)
{
	glBegin(GL_POLYGON); // Circulo
	for (int i = 0; i < 20; i++)
	{
		float angle = 2 * M_PI * i / 20;
		glVertex2f(c[0] + r * cos(angle), c[1] + r * sin(angle));
	}
	glEnd();
}

/**********************************************************************/
/*                                                                    */
/*                         Definicao de funcoes de ação               */
/*                                                                    */
/**********************************************************************/


void alfa_decay(void)
{
	if (decay[atom_index].alfa > INV)
	{
		printf("\nDecaimento Alfa\t");
		atom_index = decay[atom_index].alfa;
		setElectronicLayers();
		loadTexture();
	}
}

void beta_decay(void)
{
	if (decay[atom_index].beta > INV)
	{
		printf("\nDecaimento Beta\t");
		atom_index = decay[atom_index].beta;
		setElectronicLayers();
		loadTexture();
	}
}

/**********************************************************************/
/*                                                                    */
/*                         Definicao de funcoes auxiliares            */
/*                                                                    */
/**********************************************************************/


bool closer_than(const GLfloat c_A[2], const GLfloat c_B[2], const GLfloat d)
{
	GLfloat delta_x = c_A[0] - c_B[0];
	GLfloat delta_y = c_A[1] - c_B[1];
	return d * d >= delta_x * delta_x + delta_y * delta_y;
}


void render_text(const char *text, int x, int y)
{
	glRasterPos3i(x, y, 1);

	for (int i = 0; text[i] != '\0'; ++i)
	{

		if (text[i] == '\n')
		{ // caso de quebra de linha
			y = y - 13;
			glRasterPos3i(x, y, 1);
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	}
}


void loadTexture(void)
{
	char imgPath[255] = "./imgs/%d.png";
	sprintf(imgPath, imgPath, decay[atom_index].protons);

	printf("%s\t", imgPath);

	texture = SOIL_load_OGL_texture(imgPath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}


/// @brief Função que calcula a quantidade de elétrons em cada camada da eletrosfera, seguindo o modelo atômico de Bohr
/// @return Vetor com o número de eletrons em cada indice, correspondendo às camadas em ordem (K, L, M, ...)
void setElectronicLayers(void)
{

	int Z = decay[atom_index].protons;

	// Diagrama de Linus Pauling: "1s, 2s, 2p, 3s, 3p, 4s, 3d, 4p, 5s, 4d, 5p, 6s, 4f, 5d, 6p, 7s, 5f, 6d, 7p"

	int _1s = 2, _2s = 2, _2p = 6, _3s = 2, _3p = 6, _4s = 2, _3d = 10, _4p = 6, _5s = 2, _4d = 10, _5p = 6, _6s = 2, _4f = 14, _5d = 10, _6p = 6, _7s = 2, _5f = 14, _6d = 10, _7p = 6;

	for (int i = 0; i < sizeof(camadas) / sizeof(camadas[0]); i++)
		camadas[i] = 0;
	layer_counter = 0;

	for (int i = 0; i < Z; i++)
	{
		if (_1s > 0)
		{
			camadas[ElectronicLayers::K]++;
			_1s--;
		}
		else if (_2s > 0)
		{
			camadas[ElectronicLayers::L]++;
			_2s--;
		}
		else if (_2p > 0)
		{
			camadas[ElectronicLayers::L]++;
			_2p -= 1;
		}
		else if (_3s > 0)
		{
			camadas[ElectronicLayers::M]++;
			_3s--;
		}
		else if (_3p > 0)
		{
			camadas[ElectronicLayers::M]++;
			_3p -= 1;
		}
		else if (_4s > 0)
		{
			camadas[ElectronicLayers::N]++;
			_4s -= 1;
		}
		else if (_3d > 0)
		{
			camadas[ElectronicLayers::M]++;
			_3d -= 1;
		}
		else if (_4p > 0)
		{
			camadas[ElectronicLayers::N]++;
			_4p -= 1;
		}
		else if (_5s > 0)
		{
			camadas[ElectronicLayers::O]++;
			_5s -= 1;
		}
		else if (_4d > 0)
		{
			camadas[ElectronicLayers::N]++;
			_4d -= 1;
		}
		else if (_5p > 0)
		{
			camadas[ElectronicLayers::O]++;
			_5p -= 1;
		}
		else if (_6s > 0)
		{
			camadas[ElectronicLayers::P]++;
			_6s -= 1;
		}
		else if (_4f > 0)
		{
			camadas[ElectronicLayers::N]++;
			_4f -= 1;
		}
		else if (_5d > 0)
		{
			camadas[ElectronicLayers::O]++;
			_5d -= 1;
		}
		else if (_6p > 0)
		{
			camadas[ElectronicLayers::P]++;
			_6p -= 1;
		}
		else if (_7s > 0)
		{
			camadas[ElectronicLayers::Q]++;
			_7s -= 1;
		}
		else if (_5f > 0)
		{
			camadas[ElectronicLayers::O]++;
			_5f -= 1;
		}
		else if (_6d > 0)
		{
			camadas[ElectronicLayers::P]++;
			_6d -= 1;
		}
		else if (_7p > 0)
		{
			camadas[ElectronicLayers::Q]++;
			_7p -= 1;
		}
	}

	// printf("[ ");
	for (int i = 0; i < sizeof(camadas) / sizeof(camadas[0]); i++)
	{
		// printf("%d  ", camadas[i]);
		if (camadas[i] != 0)
			layer_counter++;
	}
	// printf("]\t");

	current_nuclear_radius = not_fermi_radius * cbrt(decay[atom_index].protons + decay[atom_index].neutrons);

	// current_valency_radius = 5.8 * pow(layer_counter,2.0)/(decay[atom_index].protons);

	// return camadas;
}
