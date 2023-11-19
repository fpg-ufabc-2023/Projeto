/*
 * projeto.cpp
 * gcc projeto.cpp -o /tmp/temp.run -lm -lglut -lGL -lGLU && /tmp/temp.run
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>     // needed to sleep


/**********************************************************************/
/*                       Declaração de variáveis                      */
/**********************************************************************/

static GLfloat spin = 0.0;
const GLint proton_number = 20;
const GLint neutron_number = 20;

/*  Cores Prótons  */
const GLfloat proton[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat proton_ambient[] = { 0.8, 0.0, 0.0, 1.0 };
const GLfloat proton_emission[] = {0.1, 0.0, 0.0, 1.0};

/*  Cores Elétrons  */
const GLfloat eletron[] = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat eletron_ambient[] = {0.0, 0.0, 0.8, 1.0};
const GLfloat eletron_emission[] = {0.0, 0.0, 0.1, 1.0};

/*  Cores Neutrons  */
const GLfloat neutron[] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat neutron_ambient[] = {0.4, 0.4, 0.4, 1.0};
const GLfloat neutron_emission[] = {0.05, 0.05, 0.05, 1.0};

const GLfloat high_shininess[] = { 100.0 };

/**********************************************************************/
/*                        Declaração de funções                       */
/**********************************************************************/

void init_glut(const char *window_name, int *argcp, char **argv);
void display_callback(void);
void reshape_callback(int w, int h);
void animate_callback(void);
void keyboard_callback(unsigned char key, int x, int y);
void keyboard_callback_special(int key, int x, int y);
void mouse_callback(int button, int state, int x, int y);
void menu_callback(int value);
void draw_particle(void);
void draw_eletron(void);
void draw_proton(void);
void draw_neutron(void);


/**********************************************************************/
/*                                                                    */
/*                       Função principal (main)                      */
/*                                                                    */
/**********************************************************************/


int main(int argc, char **argv)
{

	/* inicia o GLUT e alguns parâmetros do OpenGL */
	init_glut("Projeto KHAAP.cpp", &argc, argv);

	/* função de controlo do GLUT */
	glutMainLoop();

	return 0;
}


/**********************************************************************/
/*                                                                    */
/*                         Defini��o de fun��es                       */
/*                                                                    */
/**********************************************************************/

/*
 * inicia o GLUT e alguns par�metros do OpenGL
 */
void init_glut(const char *nome_janela, int *argcp, char **argv)
{

	/* inicia o GLUT */
	glutInit(argcp, argv);

	/* inicia o display usando RGB e double-buffering */
   	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(nome_janela);

	/* define as funções de callback */
	glutKeyboardFunc(keyboard_callback);
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutSpecialFunc(keyboard_callback_special);
	glutMouseFunc(mouse_callback);
	glutIdleFunc(animate_callback);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.0, 1.0, -1.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat local_view[] = { 0.0 };

	/* define a cor com a qual a tela será apagado */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	// glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	// glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do

	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);

	// glMatrixMode(GL_PROJECTION);

	/* define a cor de desenho inicial (azul) */
	glColor3f(0.0, 0.0, 1.0);

	return;
}

void display_callback(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		for(int i = 0; i < proton_number; i++){
			glPushMatrix();
				glRotatef ((GLfloat) i * 360.0 / proton_number , 0.0, 1.0, 0.0);
				glTranslatef (0.6-0.3*(i%5), 0.0, 0.0);
				draw_proton();
			glPopMatrix();
		}

		for(int i = 0; i < neutron_number; i++){
			glPushMatrix();
				glRotatef ((GLfloat) (i * 360.0 / neutron_number + 180.0/proton_number) , 0.0, 1.0, 0.0);
				glTranslatef (0.4+0.2*(i%3), 0.0, 0.0);
				draw_neutron();
			glPopMatrix();
		}

		for(int i = 0; i < proton_number; i++){
			glPushMatrix();
				glRotatef ((GLfloat) spin + i * 360.0 / proton_number, 0.0, 1.0, 0.0);
				glTranslatef (2.0+0.2*(i%2), 0.0, 0.0);
				draw_eletron();
			glPopMatrix();
		}
	glPopMatrix();
	glutSwapBuffers();
}

void draw_particle(void){
	glutSolidSphere(0.2, 10, 8);
}

void draw_eletron(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT, eletron_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, eletron);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, eletron_emission);
	draw_particle();
}

void draw_proton(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT, proton_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, proton);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, proton_emission);
	draw_particle();
}

void draw_neutron(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT, neutron_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, neutron);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, neutron_emission);
	draw_particle();
}

/*
 * Processa o reshape da janela
 */
void reshape_callback(int w, int h)
{

	/* define a zona da janela onde se vai desenhar */
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	/* muda para o modo GL_PROJECTION e reinicia a projec��o */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* adapta a projec��o ortogonal de acordo com as dimens�es da janela */
	if (h > w)
	{
		gluOrtho2D(-1.0, 1.0, 1.0*((GLfloat)-h / (GLfloat)w), 1.0*((GLfloat)h / (GLfloat)w));
	}
	else
	{
		gluOrtho2D(1.0*((GLfloat)-w / (GLfloat)h), 1.0*((GLfloat)w / (GLfloat)h), -1.0, 1.0);
	}

   	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);

	/* muda para o modo GL_MODELVIEW (n�o pretendemos alterar a projec��o *
	 * quando estivermos a desenhar na tela)                              */
	glMatrixMode(GL_MODELVIEW);


	glLoadIdentity();
	gluLookAt (0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	return;
}

/*
 * Fun��o necess�ria para a anima��o
 */
void animate_callback(void)
{
	glutPostRedisplay(); /* Manda o redesenhar o ecr� em cada frame */
	return;
}

/*
 * Controlo das teclas normais
 */
void keyboard_callback(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0); /* Esc: sai do programa */
	return;
}

/*
 * Controlo das teclas especiais (Cursores, F1 a F12, etc...)
 */
void keyboard_callback_special(int key, int x, int y)
{
	switch (key)
	{
	case 1:
		glColor3f(0.0, 0.0, 1.0); /* F1: muda a cor para azul */
		break;

	case 2:
		glColor3f(0.0, 1.0, 0.0); /* F2: muda a cor para verde */
		break;

	case 3:
		glColor3f(1.0, 0.0, 0.0); /* F2: muda a cor para vermelho */
		break;

	default: /* isto aqui n�o faz nada, mas evita warnings :P */
		break;
	}

	return;
}

void spinDisplay(void)
{
	spin = spin + 0.5;
	if (spin > 360.0) spin = spin - 360.0;
	glutPostRedisplay();
    usleep(100);
}

void mouse_callback(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(spinDisplay);
		break;
	case GLUT_MIDDLE_BUTTON:
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(glutPostRedisplay);
		break;
	default:
		break;
	}
}

