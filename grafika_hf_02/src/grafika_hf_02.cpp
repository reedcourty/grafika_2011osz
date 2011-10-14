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

// Osztalydefiniciok:

class Vector2D {
	private:
		float x, y;
	public:
		Vector2D() { x = y = 0.00; }

		Vector2D(float _x, float _y) {
			this->x = _x;
			this->y = _y;
		}

		Vector2D operator+(const Vector2D& v) {
			float X = this->x + v.x;
			float Y = this->y + v.y;
			return Vector2D(X, Y);
		}

		void operator+=(const Vector2D& v) {
			this->x+=v.x;
			this->y+=v.y;
		}

		Vector2D operator-(const Vector2D& v) {
			float X = this->x - v.x;
			float Y = this->y - v.y;
			return Vector2D(X, Y);
		}

		void operator-=(const Vector2D& v) {
			this->x-=v.x;
			this->y-=v.y;
		}

		Vector2D operator*(float f) {
			return Vector2D(this->x * f, this->y * f);
		}

		void operator*=(float f) {
			this->x*=f;
			this->y*=f;
		}

		float operator*(const Vector2D& v) {
			return (this->x * v.x + this->y * v.y);
		}

		Vector2D operator/(float f) {
			return Vector2D(x/f, y/f);
		}

		float Length() {
			return (float)sqrt(this->x * this->x + this->y * this->y);
		}

		void Normalize() {
			float l = Length();
			if (l < 0.000001f) {
				this->x = 1;
				this->y = 0;
			}
			else {
				x /= l;
				y /= l;
			}
		}

		Vector2D UnitVector() {
			Vector2D r = * this;
			r.Normalize();
			return r;
		}

		float& X() { return this->x; }

		float& Y() { return this->y; }

};

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
				x /= len;
				y /= len;
				z /= len;
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

const int BEZIER_VEZERLOPONTOK_SZAMA = 4;
const int BVSZ = BEZIER_VEZERLOPONTOK_SZAMA;

const int CATMULLROM_VEZERLOPONTOK_SZAMA = 9;
const int CRVSZ = CATMULLROM_VEZERLOPONTOK_SZAMA;

class BezierGorbe {
	private:
		Vector2D vezerlopontok[BVSZ+1];
		int mvpsz;

		float B(int i, float t) {
			float choose = 1;
			for (int j = 1; j <= i; j++) {
				choose *= (float)(BVSZ-i+j)/j;
			}
			return choose * pow(t, i) * pow(1-t, BVSZ-i);
		}

	public:
		BezierGorbe() {
			this->mvpsz = 0;
		}

		Vector2D r(float t) {
			Vector2D rr(0,0);
			for (int i = 0; i <= BVSZ; i++) {
				rr += this->vezerlopontok[i] * B(i,t);
			}
			return rr;
		}

		int VezerlopontHozzaadasa(Vector2D v) {
			if (this->mvpsz < BVSZ) {
				this->vezerlopontok[this->mvpsz] = v;
				this->mvpsz++;
				return 0;
			}
			else {
				return -1;
			}
		}

		void Rajzol() {
			#if defined(DEBUG)
				/* A kontrollpontok kirajzolasa: */
				glColor3f(0.5f,1.0f,0.56f);
				for (int i = 0; i <= BVSZ; i++) {
					cout << this->vezerlopontok[i].X() << "," << this->vezerlopontok[i].Y() << endl;
					glBegin(GL_POLYGON);
					glVertex2f(this->vezerlopontok[i].X()-0.01, this->vezerlopontok[i].Y()+0.01);
					glVertex2f(this->vezerlopontok[i].X()+0.01, this->vezerlopontok[i].Y()+0.01);
					glVertex2f(this->vezerlopontok[i].X()+0.01, this->vezerlopontok[i].Y()-0.01);
					glVertex2f(this->vezerlopontok[i].X()-0.01, this->vezerlopontok[i].Y()-0.01);
					glEnd();
				}
			#endif

			glColor3f(0.72157f,0.62353f,0.61961f);
			glBegin(GL_LINE_STRIP);
				for (float i = 0; i <= 1; i = i + 0.0001) {
					glVertex2f(r(i).X(), r(i).Y());
				}
				glEnd();
		}


};

class CatmullRomGorbe {
	private:
		Vector2D vp[CRVSZ];

		Vector2D a[CRVSZ];
		Vector2D b[CRVSZ];
		Vector2D c[CRVSZ];
		Vector2D d[CRVSZ];
		Vector2D v[CRVSZ];

		float ta[CRVSZ];

	public:
		/*
		 * Dr. Szirmay-Kalos L. - Haromdimenzios grafika, ... 327-330. old. alapjan
		 * f(t) = a[i]*(t-t[i])^3 + b[i]*(t-t[i])^2 + c[i]*(t-t[i]) + d[i], ha t[i] <= t < t[i+1]
		 *
		 * f[i] = f(t[i]) = d[i]
		 * f[i+1] = f(t[i+1]) = a[i]*(t[i+1]-t[i])^3 + b[i]*(t[i+1]-t[i])^2 + c[i]*(t[i+1]-t[i]) + d[i]
		 *
		 * v[i] = f'(t[i]) = c[i]
		 * v[i+1] = f'(t[i+1]) = 3*a[i]*(t[i+1]-t[i]) + 2*b[i]*(t[i+1]-t[i]) + c[i]
		 *
		 * a[i] = ((v[i+1] + v[i]) / (t[i+1]-t[i])^2) - (2*(f[i+1] - f[i]) / (t[i+1]-t[i])^3)
		 *
		 * b[i] = (3*(f[i+1] - f[i]) / (t[i+1]-t[i])^2) - ((v[i+1] + 2*v[i]) / (t[i+1]-t[i]))
		 *
		 * c[i] = v[i]
		 *
		 * d[i] = f[i]
		 *
		 * v[i] = 0.5 * ((f[i]-f[i-1])/(t[i]-t[i-1]) + (f[i+1]-f[i])/(t[i+1]-t[i]))
		 *
		 */

		void LoadTa() {
			for (int i = 0; i < CRVSZ; i++) {
				ta[i] = i * (1.0 / CRVSZ);
			}
		}

		Vector2D r(float t, int i) {
			return ai(i)*pow((t-ta[i]),3) + bi(i)*pow((t-ta[i]),2) + ci(i)*(t-ta[i]) + di(i);
		}

		void LoadA() {
			for (int i = 0; i < CRVSZ; i++) {
				a[i] = ((vi(i+1) + vi(i)) / pow((ti(i+1)-ti(i)),2)) - ((fi(i+1) - fi(i))*2 / pow((ti(i+1)-ti(i)),3));
			}
		}

		Vector2D ai(int i) { return a[i]; }

		void LoadB() {
			for (int i = 0; i < CRVSZ; i++) {
				b[i] = ((fi(i+1) - fi(i))*3 / pow((ti(i+1)-ti(i)),2)) - ((vi(i+1) + vi(i)*2) / (ti(i+1)-ti(i)));
			}
		}

		Vector2D bi(int i) { return b[i]; }

		Vector2D ci(int i) { return vi(i); }

		Vector2D di(int i) { return vp[i]; }

		Vector2D fi(int i) {
			Vector2D result;
			if ((i >= 0) && (i < CRVSZ)) { result = this->vp[i]; }
			else {
				if (i < 0) { result = vp[CRVSZ+i]; }
				if (i >= CRVSZ) { result = vp[i-CRVSZ]; }
			}
			return result;
		}

		float ti(int i) {
			if ((i >= 0) && (i < CRVSZ)) { return ta[i]; }
			else {
				if (i < 0) { return ta[CRVSZ+i]; }
				if (i >= CRVSZ) { return ta[i-CRVSZ]; }
			}
		}

		void LoadV() {
			for (int i = 0; i < CRVSZ; i++) {
				v[i] = ((fi(i)-fi(i-1))/(ti(i)-ti(i-1)) + (fi(i+1)-fi(i))/(ti(i+1)-ti(i))) * 0.5;
			}
		}

		Vector2D vi(int i) { return v[i]; }

		CatmullRomGorbe() {

			vp = { Vector2D(+0.00,+0.50),
				   Vector2D(-0.05,+0.55),
				   Vector2D(-0.20,+0.75),
				   Vector2D(-0.20,+0.50),
				   Vector2D(+0.00,-0.75),
				   Vector2D(+0.20,+0.50),
				   Vector2D(+0.20,+0.75),
				   Vector2D(+0.05,+0.55),
				   Vector2D(+0.00,+0.50) };

			LoadTa();
			LoadV();
			LoadA();
			LoadB();
		}

		void Rajzol() {

			/* A kontrollpontok kirajzolasa: */
			glColor3f(1.00f,0.00f,0.00f);
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			for (int i = 0; i < CRVSZ; i++) {
				glVertex2f(this->vp[i].X(), this->vp[i].Y());
			}
			glEnd();

			glColor3f(1.00f,.00f,1.00f);
			glPointSize(2.5f);
			glBegin(GL_POINTS);
			for (int i = 0; i < CRVSZ; i++) {
				for (float t = ti(i); t < ti(i+1); t = t + 0.00001) {
					glVertex2f(r(t,i).X(), r(t,i).Y());
				}
			}
			glEnd();

		}

};


//BezierGorbe bg;
CatmullRomGorbe cg;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) { 

	// TODO: Megnezni, hogy mekkora lehet a vezerlopontok tombje
	// TODO: Limitalni, hogy nem csorduljon tul a vezerlopontok tombje

	//bg.VezerlopontHozzaadasa(Vector2D(-0.50, +0.50));
	//bg.VezerlopontHozzaadasa(Vector2D(-0.25, +0.50));
	//bg.VezerlopontHozzaadasa(Vector2D(+0.00, +0.45));
	//bg.VezerlopontHozzaadasa(Vector2D(-0.30, +0.23));
	//bg.VezerlopontHozzaadasa(Vector2D(-0.75, +0.80));
	//bg.VezerlopontHozzaadasa(Vector2D(-0.50, +0.50));
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    // ...
    //bg.Rajzol();
    cg.Rajzol();


    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

	#if defined(DEBUG)
	PrintPressedKey(key, x, y);
	PrintTime();
	#endif

	if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN);  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
	//long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

	//glutPostRedisplay();

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

