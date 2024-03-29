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

enum liftallapot {MOZOG_FEL, MOZOG_LE, SZINTEN_ALL};

const float SZINT1 = -0.50;
const float SZINT2 = +0.50;

const float szintek[4] = { -1.0, SZINT1, SZINT2, 1.0 };

const float lift_sebesseg = 0.002;

enum allapottipus {FEJELORE, ALL, FAROKELORE};
enum iranytipus {JOBBRA, BALRA};

// A piros giliszta tulajdonsagai:
float p_giliszta_cx = +0.00;
float p_giliszta_cy = -0.40;
float p_giliszta_vx = -0.001;
float p_giliszta_vy = +0.00;

// A zold giliszta tulajdonsagai:
float z_giliszta_cx = -0.40;
float z_giliszta_cy = -0.40;
float z_giliszta_vx = -0.001;
float z_giliszta_vy = +0.00;

float giliszta_fej = 0.025;

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
		float cx, cy, szelesseg;

	public:

		MyPolygon() {

		}

		MyPolygon(float _cx, float _cy, float _szelesseg) {
			cx = _cx;
			cy = _cy;
			szelesseg = _szelesseg;
		}

		float get_szelesseg(void) {
			return this->szelesseg;
		}

		void draw(void) {
			glBegin(GL_POLYGON);
				glVertex2f(this->cx, this->cy-this->szelesseg);
				glVertex2f(this->cx-this->szelesseg, this->cy);
				glVertex2f(this->cx, this->cy+this->szelesseg);
				glVertex2f(this->cx+this->szelesseg, this->cy);
			glEnd();
		}
};

const float PI = 3.14159;
const int BONTAS = 100;

float giliszta_Amax = 0.10;

class MySine {

	// k = omega / c
	// k = (2*pi*f) / c
	// k = (2*pi) / lambda

	private:
    
        float cx, cy, Amax, Aakt, omega, phi, f, hosszmax, hosszakt;
		iranytipus irany;
		typedef float koordinata[2];
		koordinata koordinatak[BONTAS];
		float hossz_allando;

	public:

		MySine() {
			MySine(cx, cy, giliszta_Amax, 0, 0.02, 40, this->irany);
		}

		MySine(float _cx, float _cy, float _Amax, float _phi, float _f, float _hosszmax, iranytipus _irany) {
			this->cx = _cx;
			this->cy = _cy;
			this->Amax = _Amax;
			this->Aakt = _Amax;
			this->phi = _phi;
			this->f = _f;
			this->omega = 2*PI*this->f;
			this->hosszmax = _hosszmax;
			this->hosszakt = _hosszmax;
			this->irany = _irany;

			this->hossz_allando = 8*sqrt(this->Amax*this->Amax+(this->hosszmax/8)*(this->hosszmax/8));

			for (int i = 0; i <= BONTAS; i++) {
				this->koordinatak[i][0] = (float)i*(4*PI/BONTAS);
				this->koordinatak[i][1] = sin(omega*i + phi); // "A" nem itt, mert akkor mindig ujra kellene szamolni

				//#if defined(DEBUG)
				//cout << "i: " << i << " x: " << this->koordinatak[i][0] << " y: " << this->koordinatak[i][1] << endl;
				//#endif
			}
		}

		iranytipus get_irany(void) {
			return this->irany;
		}

		void set_irany(iranytipus _irany) {
			this->irany = _irany;
		}

		void draw(void) {
			glBegin(GL_LINES);
			if (irany == BALRA) {
				for (int i = 0; i < BONTAS-1; i++) {
					glVertex2f(this->koordinatak[i][0]/this->hosszakt+this->cx,this->Aakt*this->koordinatak[i][1]+this->cy);
					glVertex2f(this->koordinatak[i+1][0]/this->hosszakt+this->cx,this->Aakt*this->koordinatak[i+1][1]+this->cy);
				}
			}
			if (irany == JOBBRA) {
				for (int i = 0; i < BONTAS-1; i++) {
					glVertex2f(-1*(this->koordinatak[i][0]/this->hosszakt)+this->cx,this->Aakt*this->koordinatak[i][1]+this->cy);
					glVertex2f(-1*(this->koordinatak[i+1][0]/this->hosszakt)+this->cx,this->Aakt*this->koordinatak[i+1][1]+this->cy);
				}
			}
			glEnd();

		}
};

class Palya {
	public:

		void draw(void) {

			glColor3f(0.03f,0.863f,0.047f);

			MyRectangle szint1_1(-1.00,SZINT1-0.005,-0.60,SZINT1+0.005);
			MyRectangle szint1_2(-0.20,SZINT1-0.005,+0.20,SZINT1+0.005);
			MyRectangle szint1_3(+0.60,SZINT1-0.005,+1.00,SZINT1+0.005);

			szint1_1.draw();
			szint1_2.draw();
			szint1_3.draw();

			MyRectangle szint2_1(-1.00,SZINT2-0.005,-0.60,SZINT2+0.005);
			MyRectangle szint2_2(-0.20,SZINT2-0.005,+0.20,SZINT2+0.005);
			MyRectangle szint2_3(+0.60,SZINT2-0.005,+1.00,SZINT2+0.005);

			szint2_1.draw();
			szint2_2.draw();
			szint2_3.draw();
		}
};

class Lift {
	private:
		int azonosito;
		int szint;
		liftallapot allapot;
		float sebesseg;
		float magassag;

		long eltelt_ido;
		long utolso_rajzolas_ideje;

	public:
		Lift(int _azonosito) {
			this->szint = 1;
			this->azonosito = _azonosito;
			this->sebesseg = lift_sebesseg;
			this->magassag = get_szint_magassag();
			this->utolso_rajzolas_ideje = 0;
			this->allapot = SZINTEN_ALL;
		}

		int get_azonosito(void) { return this->azonosito; }

		int get_szint(void) { return this->szint; }
		void set_szint(int _szint) { this->szint = _szint; }

		liftallapot get_allapot(void) {	return this->allapot; }
		void set_allapot(liftallapot _allapot) { this->allapot = _allapot; }

		int get_sebesseg(void) { return this->sebesseg; }
		void set_sebesseg(int _sebesseg) { this->sebesseg = _sebesseg; }

		void szint_fel(void) {
			if (this->szint < 3) { this->szint++; }
		}

		void szint_le(void) {
			if (this->szint > 0) { this->szint--; }
		}

		float get_szint_magassag(void) {
			int i = this->szint;
			return szintek[i];
		}

		float get_szint_magassag(int szint) {
			return szintek[szint];
		}

		void szamol(void) {

			long time = glutGet(GLUT_ELAPSED_TIME);

			this->eltelt_ido = time - this->utolso_rajzolas_ideje;

			if (this->allapot == MOZOG_FEL) {
				if (this->magassag < get_szint_magassag(get_szint()+1)) {
					this->magassag = this->magassag + this->sebesseg * eltelt_ido;
				}
				else {
					set_allapot(SZINTEN_ALL);
					szint_fel();
					this->magassag = get_szint_magassag(get_szint());
					rajzol();
				}
			}

			if (this->allapot == MOZOG_LE) {
				if (this->magassag > get_szint_magassag(get_szint()-1)) {
					this->magassag = this->magassag - this->sebesseg * eltelt_ido;
				}
				else {
					set_allapot(SZINTEN_ALL);
					szint_le();
					this->magassag = get_szint_magassag(get_szint());
					rajzol();
				}
			}

		}

		void rajzol(void) {

			szamol();

			glColor3f(0.03f,0.53f,0.047f);

			if (this->azonosito == 1) {
				MyRectangle _lift(-0.60,this->magassag-0.005,-0.20,this->magassag+0.005);
				_lift.draw();
			}
			if (this->azonosito == 2) {
				MyRectangle _lift(+0.20,this->magassag-0.005,+0.60,this->magassag+0.005);
				_lift.draw();
			}

			this->utolso_rajzolas_ideje = glutGet(GLUT_ELAPSED_TIME);
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


		allapottipus allapot;

		iranytipus irany;

		MySine farok;

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

			allapot = ALL;
			irany = BALRA;
		}

		void szamol(void) {
			time = glutGet(GLUT_ELAPSED_TIME);

			this->eltelt_ido = time - this->utolso_rajzolas_ideje;

			if (this->allapot != ALL) {
			this->cx = this->cx + this->vx * this->eltelt_ido;
			this->cy = this->cy + this->vy * this->eltelt_ido;
			}

			if (this->cx <= -1.0+giliszta_fej) {
				this->vx = 0 + fabs(this->vx);
				this->irany = JOBBRA;
			}
			if (this->cx >= 1.0-giliszta_fej) {
				this->vx = 0 - fabs(this->vx);
				this->irany = BALRA;
			}
			if (this->cy <= -1.0+giliszta_fej) {
				this->vy = 0 + fabs(this->vy);
			}
			if (this->cy >= 1.0-giliszta_fej) {
				this->vy = 0 - fabs(this->vy);
			}

		}

		allapottipus get_allapot(void) {
			return this->allapot;
		}

		void set_allapot(allapottipus _allapot) {
			this->allapot = _allapot;
		}

		void draw(void) {

			szamol();

			glColor3f(szin_R,szin_G,szin_B);

			MyPolygon mp(cx, cy, giliszta_fej);
			mp.draw();
			MySine farok(cx, cy, giliszta_Amax, 0, 0.02, 40, this->irany);
			farok.set_irany(this->irany);
			farok.draw();

			this->utolso_rajzolas_ideje = time;
		}

};

class Gilisztapp {
	private:
		MyPolygon fej;
		MySine farok;

		iranytipus irany;

		float cx, cy;

	public:
		Gilisztapp() {
			fej = MyPolygon(cx, cy, giliszta_fej);
			farok = MySine(cx, cy, giliszta_Amax, 0, 0.02, 40, this->irany);
		}
};

Palya p;
Lift LiftQA(1);
Lift LiftOL(2);

Giliszta z_giliszta(0.55f,0.71f,0.00f,z_giliszta_cx,z_giliszta_cy,z_giliszta_vx,z_giliszta_vy);
Giliszta p_giliszta(0.64f,0.00f,0.00f,p_giliszta_cx,p_giliszta_cy,p_giliszta_vx,p_giliszta_vy);

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) { 

	glColor3f(1.0f,0.863f,0.047f);

	p_giliszta.set_allapot(ALL);
	z_giliszta.set_allapot(ALL);


}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    // ...
    p.draw();
    LiftQA.rajzol();
    LiftOL.rajzol();
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
    	if (LiftQA.get_allapot() == SZINTEN_ALL) {
    		LiftQA.set_allapot(MOZOG_FEL);
    	}
   		glutPostRedisplay();

    }
    if (key == 'a') {
    	if (LiftQA.get_allapot() == SZINTEN_ALL) {
    		LiftQA.set_allapot(MOZOG_LE);
    	}
    	glutPostRedisplay();
    }

    if (key == 'o') {
    	if (LiftOL.get_allapot() == SZINTEN_ALL) {
    	    LiftOL.set_allapot(MOZOG_FEL);
    	}
   		glutPostRedisplay();
    }
    if (key == 'l') {
    	if (LiftOL.get_allapot() == SZINTEN_ALL) {
    		LiftOL.set_allapot(MOZOG_LE);
    	}
   		glutPostRedisplay();
    }

	#if defined(DEBUG)
    	cout << "LiftQA.allapot: " << LiftQA.get_allapot() << endl;
    	cout << "LiftQA.szint: " << LiftQA.get_szint() << endl;
    	cout << "LiftOL.allapot: " << LiftOL.get_allapot() << endl;
    	cout << "LiftOL.szint: " << LiftOL.get_szint() << endl;
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

