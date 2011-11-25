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

// TODO: A beadas elott kapcsold ki a DEBUG modot:
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

const float PI = 3.14159265358979323846;

float AMBIENS_FENY[4] = {0,0,1,0};
const int HENGERVERTEXSZAM = 64;

class Vector3D {
	// Dr. Szirmay-Kalos L. - Haromdimenzios grafika, ... 35. old. alapjan
	private:
		float x, y, z;
	public:
		Vector3D() { x = y = z = 0.00; }

		Vector3D(float _x, float _y, float _z) {
			this->x = _x;
			this->y = _y;
			this->z = _z;
		}

		Vector3D operator+(const Vector3D& v) {
			float X = this->x + v.x;
			float Y = this->y + v.y;
			float Z = this->z + v.z;
			return Vector3D(X, Y, Z);
		}

		void operator+=(const Vector3D& v) {
			this->x+=v.x;
			this->y+=v.y;
			this->z+=v.z;
		}

		Vector3D operator-(const Vector3D& v) {
			float X = this->x - v.x;
			float Y = this->y - v.y;
			float Z = this->z - v.z;
			return Vector3D(X, Y, Z);
		}

		void operator-=(const Vector3D& v) {
			this->x-=v.x;
			this->y-=v.y;
			this->y-=v.z;
		}

		Vector3D operator*(float f) {
			return Vector3D(this->x * f, this->y * f, this->z * f);
		}

		void operator*=(float f) {
			this->x*=f;
			this->y*=f;
			this->z*=f;
		}

		float operator*(const Vector3D& v) {
			return (this->x * v.x + this->y * v.y + this->z * v.z);
		}

		Vector3D operator%(const Vector3D& v) {
			return Vector3D(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x);
		}

		Vector3D operator/(float f) {
			return Vector3D(x/f, y/f, z/f);
		}

		float Length() {
			return (float)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		}

		void Normalize() {
			float len = Length();

			if (len < 0.000001f) {
				this->x = 1;
				this->y = 0;
				this->z = 0;
			}
			else {
				this->x /= len;
				this->y /= len;
				this->z /= len;
			}
		}

		Vector3D UnitVector() {
			Vector3D r = * this;
			r.Normalize();
			return r;
		}

		float& X() { return this->x; }

		float& Y() { return this->y; }

		float& Z() { return this->z; }

};

class Camera {
	private:
		Vector3D eye_position;
		Vector3D look_at;
		Vector3D up;
	public:
		Camera() {
			eye_position = Vector3D(800,600,700);
			look_at = Vector3D(0,0,0);
			up = Vector3D(0,1,0);
		}

		void Init() {
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_NORMALIZE);
			glEnable(GL_LIGHTING);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(50,0,1,100);
		}
};

class Sun {
	private:
		Vector3D pozicio;
		float h;
	public:
		Sun() {
			pozicio = Vector3D(-1,1,1);
			h = 0;
		}

		Vector3D getPozicio() {
			return this->pozicio;
		}

		float getH() {
			return this->h;
		}

};

class Vertex {
	private:
		Vector3D csucspont[3];
		Vector3D normalvektor;
	public:
		Vertex() {};
		Vertex(Vector3D _csucspont[3]) {
			this->csucspont[0] = _csucspont[0];
			this->csucspont[1] = _csucspont[1];
			this->csucspont[2] = _csucspont[2];

			Vector3D v1 = csucspont[0]-csucspont[1];
			Vector3D v2 = csucspont[2]-csucspont[1];

			this->normalvektor = v1%v2;
		};

		Vertex(Vector3D &v1, Vector3D &v2, Vector3D &v3) {
			this->csucspont[0] = v1;
			this->csucspont[1] = v2;
			this->csucspont[2] = v3;

			Vector3D n1 = csucspont[0]-csucspont[1];
			Vector3D n2 = csucspont[2]-csucspont[1];

			this->normalvektor = n1%n2;
		}

		void Rajzol() {

			#if defined(DEBUG)
				cout << "1. csucspont: " << csucspont[0].X() << "," << csucspont[0].Y() << "," << csucspont[0].Z() << endl;
				cout << "2. csucspont: " << csucspont[1].X() << "," << csucspont[1].Y() << "," << csucspont[1].Z() << endl;
				cout << "3. csucspont: " << csucspont[2].X() << "," << csucspont[2].Y() << "," << csucspont[2].Z() << endl;
				cout << "Normalvektor: " << normalvektor.X() << "," << normalvektor.Y() << "," << normalvektor.Z() << endl;
			#endif

			glBegin(GL_TRIANGLES);
				glNormal3f(normalvektor.X(),normalvektor.Y(),normalvektor.Z());
				glTexCoord2d(0,0);
				glVertex3f(csucspont[0].X(),csucspont[0].Y(),csucspont[0].Z());
				glTexCoord2d(1,1);
				glVertex3f(csucspont[1].X(),csucspont[1].Y(),csucspont[1].Z());
				glTexCoord2d(0,0);
				glVertex3f(csucspont[2].X(),csucspont[2].Y(),csucspont[2].Z());
			glEnd();
		}
};

float fok = 0;

class Teglatest {
	private:
		Vector3D csucspontok[8];
		Vertex vertexek[12];
	public:
		Teglatest(Vector3D _csucspontok[8]) {
			for (int i = 0; i < 8; i++) {
				this->csucspontok[i] = _csucspontok[i];
			}

			Vertex v;
			v = Vertex(csucspontok[0], csucspontok[1], csucspontok[2]);
			this->vertexek[0] = v;

			v = Vertex(csucspontok[0], csucspontok[2], csucspontok[3]);
			this->vertexek[1] = v;

			v = Vertex(csucspontok[1], csucspontok[5], csucspontok[6]);
			this->vertexek[2] = v;

			v = Vertex(csucspontok[1], csucspontok[6], csucspontok[2]);
			this->vertexek[3] = v;

			v = Vertex(csucspontok[5], csucspontok[4], csucspontok[7]);
			this->vertexek[4] = v;

			v = Vertex(csucspontok[5], csucspontok[7], csucspontok[6]);
			this->vertexek[5] = v;

			v = Vertex(csucspontok[4], csucspontok[0], csucspontok[3]);
			this->vertexek[6] = v;

			v = Vertex(csucspontok[4], csucspontok[3], csucspontok[7]);
			this->vertexek[7] = v;

			v = Vertex(csucspontok[0], csucspontok[4], csucspontok[5]);
			this->vertexek[8] = v;

			v = Vertex(csucspontok[0], csucspontok[5], csucspontok[1]);
			this->vertexek[9] = v;

			v = Vertex(csucspontok[3], csucspontok[7], csucspontok[6]);
			this->vertexek[10] = v;

			v = Vertex(csucspontok[3], csucspontok[2], csucspontok[2]);
			this->vertexek[11] = v;

		}

		void Rajzol() {

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotatef(fok,0.45,1,0);

			for (int i = 0; i < 12; i++) {
				vertexek[i].Rajzol();
			}
			glLoadIdentity();
		}
};

class Henger {
	private:
		float magassag;
		float sugar;
		Vector3D kozeppont;

		Vertex vertexek[HENGERVERTEXSZAM*3];
	public:
		Henger() {
			magassag = 0.5;
			sugar = 0.5;
			kozeppont = Vector3D(0,0,0);
		};

		void ComputeVertices() {
			Vector3D teteje = Vector3D(kozeppont.X(),kozeppont.Y()+magassag/2,kozeppont.Z());
			Vector3D alja = Vector3D(kozeppont.X(),kozeppont.Y()-magassag/2,kozeppont.Z());

			Vector3D elsopont = Vector3D(kozeppont.X(),kozeppont.Y()+magassag/2,kozeppont.Z()+sugar);

			//#if defined(DEBUG)
			//	cout << "teteje: " << teteje.X() << "," << teteje.Y() << "," << teteje.Z() << endl;
			//	cout << "alja: " << alja.X() << "," << alja.Y() << "," << alja.Z() << endl;
			//	cout << "elsopont: " << elsopont.X() << "," << elsopont.Y() << "," << elsopont.Z() << endl;
			//#endif

			Vector3D felsolap[HENGERVERTEXSZAM+1], alsolap[HENGERVERTEXSZAM+1];
			float x, z, szog;

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				szog = 2*PI/(HENGERVERTEXSZAM/2)*i;
				x = sugar*cos(szog);
				z = sugar*sin(szog);
				felsolap[i] = Vector3D(x,teteje.Y(),z);
			}

			felsolap[HENGERVERTEXSZAM/2] = felsolap[0];

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[i] = Vertex(teteje,felsolap[i],felsolap[i+1]);
			}

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				szog = 2*PI/(HENGERVERTEXSZAM/2)*i;
				x = sugar*cos(szog);
				z = sugar*sin(szog);
				alsolap[i] = Vector3D(x,alja.Y(),z);
			}

			alsolap[HENGERVERTEXSZAM/2] = alsolap[0];

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[i+HENGERVERTEXSZAM/2] = Vertex(alja,alsolap[i],alsolap[i+1]);
			}

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[i+HENGERVERTEXSZAM] = Vertex(felsolap[i],felsolap[i+1],alsolap[i]);
			}

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[i+HENGERVERTEXSZAM*2] = Vertex(felsolap[i+1],alsolap[i+1],alsolap[i]);
			}

		}

		void Rajzol() {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotatef(fok,1,0,0);
			for (int i = 0; i < HENGERVERTEXSZAM*3; i++) {
				vertexek[i].Rajzol();
			}
			glLoadIdentity();
		}
};

Vector3D csucsok[8]={
		Vector3D(+0.00,+0.00,+0.00),
		Vector3D(+0.50,+0.00,+0.00),
		Vector3D(+0.50,-0.50,+0.00),
		Vector3D(+0.00,-0.50,+0.00),
		Vector3D(+0.00,+0.00,+0.50),
		Vector3D(+0.50,+0.00,+0.50),
		Vector3D(+0.50,-0.50,+0.50),
		Vector3D(+0.00,-0.50,+0.50),
};

//Teglatest t(csucsok);
Henger henger;

Camera camera;
Sun sun;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	camera.Init();
	henger.ComputeVertices();
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {

	glViewport(0,0,600,600);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles


    glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

    	gluLookAt(30*sin(fok),30*cos(fok),300,0,0,0,0,1,0);

    		glLightfv(GL_LIGHT0,GL_POSITION,AMBIENS_FENY);
    		glEnable(GL_LIGHT0);

    		float nap[4] = {sun.getPozicio().X(), sun.getPozicio().Y(), sun.getPozicio().Z(), sun.getH()};
    		glLightfv(GL_LIGHT1,GL_POSITION,nap);

    		glEnable(GL_LIGHT1);

    //t.Rajzol();
    henger.Rajzol();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

	#if defined(DEBUG)
	PrintPressedKey(key, x, y);
	PrintTime();
	#endif

    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

    if (key == 'w') {
    	fok += 1;
    	glutPostRedisplay();
    }

    if (key == 's') {
        fok -= 1;
        glutPostRedisplay();
    }

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
     // long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
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

