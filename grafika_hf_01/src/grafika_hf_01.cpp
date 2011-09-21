//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2011-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Nadudvari Gyorgy
// Neptun : ULQP9P
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#include <math.h>
#include <stdlib.h>

// TODO: A beadas elott torold ki a kovetkezo sorokat:
#define DEBUG

#if defined(DEBUG)
#include <iostream>
using namespace std;
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>     
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
// A GLUT-ot le kell tolteni: http://www.opengl.org/resources/libraries/glut/
#include <GL/glut.h>     

#define new new_nelkul_is_meg_lehet_csinalni

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

// A DEBUG-olast segito fuggvenyek, metodusok:

#if defined(DEBUG)

void PrintPressedKey(unsigned char key, int x, int y) {
	cout << "Billentyuzet: gomb: " << key << " Eger koordinatak: x: " << x << " y: " << y << endl;
}

void PrintTime() {
	long time = glutGet(GLUT_ELAPSED_TIME);
	cout << "Eltelt ido: " << time << "ms" << endl;
}

#endif

const float SZINT1 = +0.50;
const float SZINT2 = -0.50;

const float szintek[4] = { -1.0, SZINT1, SZINT2, 1.0 };

// A liftek tulajdonsagai:
// A QA lift kezdeti magassaga:
float LiftQA_kezdeti_magassag = 0;
// Az OL lift kezdeti magassaga:
float LiftOL_kezdeti_magassag = 0;


// A piros giliszta tulajdonsagai:
float p_giliszta_cx = +0.00;
float p_giliszta_cy = +0.00;
float p_giliszta_vx = +0.001;
float p_giliszta_vy = -0.001;

// A zold giliszta tulajdonsagai:
float z_giliszta_cx = -0.40;
float z_giliszta_cy = -0.40;
float z_giliszta_vx = +0.001;
float z_giliszta_vy = -0.001;

float giliszta_fej = 0.10;

class MyRectangle {
	private:
		float x1, y1, x2, y2;

	public:

		MyRectangle(float _x1, float _y1, float _x2, float _y2) {
			x1 = _x1;
			y1 = _y1;
			x2 = _x2;
			y2 = _y2;
		}

		void draw(void) {
			glBegin(GL_POLYGON);
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);
				glVertex2f(x2, y2);
				glVertex2f(x1, y2);
			glEnd();
		}
};

class MyPolygon {
	private:
		float cx, cy;

	public:

		MyPolygon(float _cx, float _cy) {
			cx = _cx;
			cy = _cy;
		}

		void draw(void) {
			glBegin(GL_POLYGON);
				glVertex2f(cx, cy-giliszta_fej);
				glVertex2f(cx-giliszta_fej, cy);
				glVertex2f(cx, cy+giliszta_fej);
				glVertex2f(cx+giliszta_fej, cy);
			glEnd();
		}
};

class Palya {
	public:
		float szint1, szint2;


	void draw(void) {

		glColor3f(0.03f,0.863f,0.047f);

		MyRectangle szint1_1(-1.00,szint1-0.005,-0.60,szint1+0.005);
		MyRectangle szint1_2(-0.20,szint1-0.005,+0.20,szint1+0.005);
		MyRectangle szint1_3(+0.60,szint1-0.005,+1.00,szint1+0.005);

		szint1_1.draw();
		szint1_2.draw();
		szint1_3.draw();

		MyRectangle szint2_1(-1.00,szint2-0.005,-0.60,szint2+0.005);
		MyRectangle szint2_2(-0.20,szint2-0.005,+0.20,szint2+0.005);
		MyRectangle szint2_3(+0.60,szint2-0.005,+1.00,szint2+0.005);

		szint2_1.draw();
		szint2_2.draw();
		szint2_3.draw();
	}
};

class Lift {
	private:
		float szint;

	public:
		float magassag;
		float szam;

		void draw(void) {
			glColor3f(0.03f,0.53f,0.047f);

			if (szam == 1) {
				MyRectangle _lift(-0.60,magassag-0.005,-0.20,magassag+0.005);
				_lift.draw();
			}
			if (szam == 2) {
				MyRectangle _lift(+0.20,magassag-0.005,+0.60,magassag+0.005);
				_lift.draw();
			}

		}
};

class Giliszta {
	private:
		float szin_R;
		float szin_G;
		float szin_B;
		float cx;
		float cy;

		float vx;
		float vy;

		long utolso_rajzolas_ideje;
		long time;
		long eltelt_ido;

	public:
		Giliszta(float _szin_R, float _szin_G, float _szin_B, float _cx, float _cy, float _vx, float _vy) {
			szin_R = _szin_R;
			szin_G = _szin_G;
			szin_B = _szin_B;
			cx = _cx;
			cy = _cy;
			vx = _vx;
			vy = _vy;
			utolso_rajzolas_ideje = 0;

			eltelt_ido = time - utolso_rajzolas_ideje;
		}

		void szamol(void) {
			time = glutGet(GLUT_ELAPSED_TIME);

			this->eltelt_ido = time - this->utolso_rajzolas_ideje;

			this->cx = this->cx + this->vx * this->eltelt_ido;
			this->cy = this->cy + this->vy * this->eltelt_ido;

			#if defined(DEBUG)
			cout << "cx: " << this->cx << " cy: " << this->cy << endl;
			#endif

			if (this->cx <= -1.0+giliszta_fej) {
				this->vx = 0 + fabs(this->vx);
			}
			if (this->cx >= 1.0-giliszta_fej) {
				this->vx = 0 - fabs(this->vx);
			}
			if (this->cy <= -1.0+giliszta_fej) {
				this->vy = 0 + fabs(this->vy);
			}
			if (this->cy >= 1.0-giliszta_fej) {
				this->vy = 0 - fabs(this->vy);
			}

		}

		void draw(void) {

			szamol();

			glColor3f(szin_R,szin_G,szin_B);

			MyPolygon mp(cx, cy);
			mp.draw();

			this->utolso_rajzolas_ideje = time;
		}

};

Palya p;
Lift LiftQA;
Lift LiftOL;
Giliszta z_giliszta(0.55f,0.71f,0.00f,z_giliszta_cx,z_giliszta_cy,z_giliszta_vx,z_giliszta_vy);
Giliszta p_giliszta(0.64f,0.00f,0.00f,p_giliszta_cx,p_giliszta_cy,p_giliszta_vx,p_giliszta_vy);

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) { 

	glColor3f(1.0f,0.863f,0.047f);

	p.szint1 = SZINT1;
	p.szint2 = SZINT2;

	LiftQA.szam = 1;
	LiftQA.magassag = LiftQA_kezdeti_magassag;

	LiftOL.szam = 2;
	LiftOL.magassag = LiftOL_kezdeti_magassag;

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    // ...
    p.draw();
    LiftQA.draw();
    LiftOL.draw();
    p_giliszta.draw();
    z_giliszta.draw();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

	#if defined(DEBUG)
	PrintPressedKey(key, x, y);
	PrintTime();
	#endif

    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

    if (key == 'q') {
    	if (LiftQA.magassag <= +1.0) {
    		LiftQA.magassag = LiftQA.magassag + 0.05;
    		glutPostRedisplay();
    	}

    }
    if (key == 'a') {
    	if (LiftQA.magassag >= -1.0) {
    		LiftQA.magassag = LiftQA.magassag - 0.05;
    		glutPostRedisplay();
    	}
    }

    if (key == 'o') {
    	if (LiftOL.magassag <= +1.0) {
    		LiftOL.magassag = LiftOL.magassag + 0.05;
    		glutPostRedisplay();
        }
    }
    if (key == 'l') {
    	if (LiftOL.magassag >= -1.0) {
    		LiftOL.magassag = LiftOL.magassag - 0.05;
    		glutPostRedisplay();
    	}
    }

	#if defined(DEBUG)
    cout << "LiftQA.magassag: " << LiftQA.magassag << endl;
    cout << "LiftOL.magassag: " << LiftOL.magassag << endl;
	#endif

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
	//long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

	glutPostRedisplay();

}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}

