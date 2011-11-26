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
const int HENGERVERTEXSZAM = 128;
const int KUPVERTEXSZAM = 128;
const int ELLIPSZOIDVERTEXSZAM = 32;

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
			gluPerspective(4,1,1,100);
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

			this->normalvektor = v2%v1;
		};

		Vertex(Vector3D &v1, Vector3D &v2, Vector3D &v3) {
			this->csucspont[0] = v1;
			this->csucspont[1] = v2;
			this->csucspont[2] = v3;

			Vector3D n1 = csucspont[0]-csucspont[1];
			Vector3D n2 = csucspont[2]-csucspont[1];

			this->normalvektor = n2%n1;
		}

		void Rajzol() {


			/*#if defined(DEBUG)
				cout << "1. csucspont: " << csucspont[0].X() << "," << csucspont[0].Y() << "," << csucspont[0].Z() << endl;
				cout << "2. csucspont: " << csucspont[1].X() << "," << csucspont[1].Y() << "," << csucspont[1].Z() << endl;
				cout << "3. csucspont: " << csucspont[2].X() << "," << csucspont[2].Y() << "," << csucspont[2].Z() << endl;
				cout << "Normalvektor: " << normalvektor.X() << "," << normalvektor.Y() << "," << normalvektor.Z() << endl;
			#endif
			*/

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


class Ojjektum {
	private:
		float forgatas_szog;
		float forgatas_x;
		float forgatas_y;
		float forgatas_z;
		Vector3D eltolas;
	protected:
		Vertex vertexek[2048];
		int vertexszam;

	public:

		Ojjektum() {
			vertexszam = 0;
			eltolas = Vector3D(0,0,0);
		}

		void setRotate(float szog, float x, float y, float z) {
			forgatas_szog = szog;
			forgatas_x = x;
			forgatas_y = y;
			forgatas_z = z;
		}

		void setEltolas(Vector3D v) {
			eltolas = v;
		}

		void Rajzol() {
			//glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
			glPushMatrix();
			glTranslatef(eltolas.X(),eltolas.Y(),eltolas.Z());
			glRotatef(forgatas_szog,forgatas_x,forgatas_y,forgatas_z);
			for (int i = 0; i < vertexszam; i++) {
				vertexek[i].Rajzol();
			}
			glPopMatrix();
			//glLoadIdentity();
		}

};

class Teglatest : public Ojjektum {
	private:
		float a;
		float b;
		float c;
		Vector3D kozeppont;
		Vector3D csucspontok[8];
	public:
		Teglatest() {}
		Teglatest(float _a, float _b, float _c, Vector3D _kozeppont) {
			this->a = _a;
			this->b = _b;
			this->c = _c;
			this->kozeppont = _kozeppont;
			ComputeCsucsok();
			ComputeVertices();
		}

		void ComputeCsucsok() {
			csucspontok[0]=Vector3D(kozeppont.X()-a/2,kozeppont.Y()+b/2,kozeppont.Z()-c/2);
			csucspontok[1]=Vector3D(kozeppont.X()+a/2,kozeppont.Y()+b/2,kozeppont.Z()-c/2);
			csucspontok[2]=Vector3D(kozeppont.X()+a/2,kozeppont.Y()-b/2,kozeppont.Z()-c/2);
			csucspontok[3]=Vector3D(kozeppont.X()-a/2,kozeppont.Y()-b/2,kozeppont.Z()-c/2);

			csucspontok[4]=Vector3D(kozeppont.X()-a/2,kozeppont.Y()+b/2,kozeppont.Z()+c/2);
			csucspontok[5]=Vector3D(kozeppont.X()+a/2,kozeppont.Y()+b/2,kozeppont.Z()+c/2);
			csucspontok[6]=Vector3D(kozeppont.X()+a/2,kozeppont.Y()-b/2,kozeppont.Z()+c/2);
			csucspontok[7]=Vector3D(kozeppont.X()-a/2,kozeppont.Y()-b/2,kozeppont.Z()+c/2);
		}

		void ComputeVertices() {
			Vertex v;
			v = Vertex(csucspontok[0], csucspontok[1], csucspontok[2]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[0], csucspontok[2], csucspontok[3]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[1], csucspontok[5], csucspontok[6]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[1], csucspontok[6], csucspontok[2]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[5], csucspontok[4], csucspontok[7]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[5], csucspontok[7], csucspontok[6]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[4], csucspontok[0], csucspontok[3]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[4], csucspontok[3], csucspontok[7]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[0], csucspontok[4], csucspontok[5]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[0], csucspontok[5], csucspontok[1]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[3], csucspontok[6], csucspontok[7]);
			this->vertexek[vertexszam] = v;
			vertexszam++;

			v = Vertex(csucspontok[3], csucspontok[2], csucspontok[6]);
			this->vertexek[vertexszam] = v;
			vertexszam++;
		}
};

class Henger : public Ojjektum {
	private:
		float magassag;
		float sugar;
		Vector3D kozeppont;

	public:
		Henger() {
			magassag = 0.5;
			sugar = 0.5;
			kozeppont = Vector3D(0,0,0);
			ComputeVertices();
		};

		Henger(float _magassag, float _sugar, Vector3D _kozeppont) {
			this->magassag = _magassag;
			this->sugar = _sugar;
			this->kozeppont = _kozeppont;
			ComputeVertices();
		}

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
				vertexek[vertexszam] = Vertex(alja,alsolap[i],alsolap[i+1]);
				vertexszam++;
			}

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[vertexszam] = Vertex(felsolap[i],felsolap[i+1],alsolap[i]);
				vertexszam++;
			}

			for (int i = 0; i < HENGERVERTEXSZAM/2; i++) {
				vertexek[vertexszam] = Vertex(felsolap[i+1],alsolap[i+1],alsolap[i]);
				vertexszam++;
			}

		}
};

class Kup : public Ojjektum {
private:
		float magassag;
		float sugar;
		Vector3D kozeppont;

	public:
		Kup() {
			magassag = 0.70;
			sugar = 0.25;
			kozeppont = Vector3D(0,0,0);
			ComputeVertices();
		};

		Kup(float _magassag, float _sugar, Vector3D _kozeppont) {
			magassag = _magassag;
			sugar = _sugar;
			kozeppont = _kozeppont;
			ComputeVertices();
		}

		void ComputeVertices() {
			Vector3D teteje = Vector3D(kozeppont.X(),kozeppont.Y()+magassag/2,kozeppont.Z());
			Vector3D alja = Vector3D(kozeppont.X(),kozeppont.Y()-magassag/2,kozeppont.Z());

			Vector3D elsopont = Vector3D(kozeppont.X(),kozeppont.Y()+magassag/2,kozeppont.Z()+sugar);

			Vector3D alsolap[KUPVERTEXSZAM+1];
			float x, z, szog;

			for (int i = 0; i < KUPVERTEXSZAM/2; i++) {
				szog = 2*PI/(KUPVERTEXSZAM/2)*i;
				x = sugar*cos(szog);
				z = sugar*sin(szog);
				alsolap[i] = Vector3D(x,alja.Y(),z);
			}

			alsolap[KUPVERTEXSZAM/2] = alsolap[0];

			for (int i = 0; i < KUPVERTEXSZAM/2; i++) {
				vertexek[vertexszam] = Vertex(alja,alsolap[i],alsolap[i+1]);
				vertexszam++;
			}

			for (int i = 0; i < KUPVERTEXSZAM/2; i++) {
				vertexek[vertexszam] = Vertex(teteje,alsolap[i+1],alsolap[i]);
				vertexszam++;
			}
		}

};

class Ellipszoid : public Ojjektum {
	private:
		float hossz;
		float sugar;
		Vector3D kozeppont;
	public:
		Ellipszoid() {
			hossz = 0.60;
			sugar = 0.25;
			kozeppont = Vector3D(0,0,0);
			ComputeVertices();
		}

		Ellipszoid(float _hossz, float _sugar, Vector3D v) {
			hossz = _hossz;
			sugar = _sugar;
			kozeppont = v;
			ComputeVertices();
		}


		void ComputeSzalag(float sugar0, float sugar1, float magassag0, float magassag1, int irany) {
			float x, z, szog;

			Vector3D kkorlap[ELLIPSZOIDVERTEXSZAM+1];
			Vector3D mkorlap[ELLIPSZOIDVERTEXSZAM+1];

			if (irany == 1) {
				magassag0 = -1*magassag0;
				magassag1 = -1*magassag1;
			}

			for (int i = 0; i < ELLIPSZOIDVERTEXSZAM; i++) {
				szog = 2*PI/(ELLIPSZOIDVERTEXSZAM)*i;
				x = sugar0*cos(szog);
				z = sugar0*sin(szog);
				kkorlap[i] = Vector3D(x,kozeppont.Y()+magassag0,z);
			}

			kkorlap[ELLIPSZOIDVERTEXSZAM] = kkorlap[0];

			for (int i = 0; i < ELLIPSZOIDVERTEXSZAM; i++) {
				szog = 2*PI/(ELLIPSZOIDVERTEXSZAM)*i;
				x = sugar1*cos(szog);
				z = sugar1*sin(szog);
				mkorlap[i] = Vector3D(x,kozeppont.Y()+magassag1,z);
			}

			mkorlap[ELLIPSZOIDVERTEXSZAM] = mkorlap[0];

			if (irany == 0) {
				for (int i = 0; i < ELLIPSZOIDVERTEXSZAM; i++) {
					vertexek[vertexszam] = Vertex(kkorlap[i],mkorlap[i],mkorlap[i+1]);
					vertexszam++;
					vertexek[vertexszam] = Vertex(kkorlap[i],mkorlap[i+1],kkorlap[i+1]);
					vertexszam++;
				}
			}
			else {
				for (int i = 0; i < ELLIPSZOIDVERTEXSZAM; i++) {
					vertexek[vertexszam] = Vertex(kkorlap[i],mkorlap[i+1],mkorlap[i]);
					vertexszam++;
					vertexek[vertexszam] = Vertex(kkorlap[i],kkorlap[i+1],mkorlap[i+1]);
					vertexszam++;
				}
			}


		}

		void ComputeVertices() {
			float sugar0;
			float sugar1;
			float magassag0;
			float magassag1;

			int szint = 10;

			sugar0 = sugar;
			magassag0 = 0;

			for (int i = 0; i < szint/2; i++) {
				sugar1 = sugar/szint*(szint-(i+1)*i*0.5);
				magassag1 = hossz/szint*(i+1)*2.5-(i+1)*0.05;

				ComputeSzalag(sugar0, sugar1, magassag0, magassag1, 0);
				ComputeSzalag(sugar0, sugar1, magassag0, magassag1, 1);

				sugar0 = sugar1;
				magassag0 = magassag1;
			}

		}

};

class Uthenger {
	private:

	public:
		Teglatest teto, ttarto1, ttarto2, ttarto3, ttarto4, kemeny, test, motor, tengely1, tengely2;
		Henger elsokerek, hatsokerek;

		Uthenger() {
			teto = Teglatest(0.8,0.1,0.6, Vector3D(0.0,0.50,0.0));
			ttarto1 = Teglatest(0.1,0.4,0.1, Vector3D(0.25,0.25,0.25));
			ttarto2 = Teglatest(0.1,0.4,0.1, Vector3D(-0.25,0.25,0.25));
			ttarto3 = Teglatest(0.1,0.4,0.1, Vector3D(0.25,0.25,-0.25));
			ttarto4 = Teglatest(0.1,0.4,0.1, Vector3D(-0.25,0.25,-0.25));

			test = Teglatest(1.2,0.2,0.8, Vector3D(0.0,0.0,0.0));

			kemeny = Teglatest(0.05,0.2,0.05, Vector3D(-0.50,0.20,-0.25));

			motor = Teglatest(0.4,0.4,0.4, Vector3D(0.0,-0.25,0.0));

			elsokerek = Henger(0.8, 0.25, Vector3D(-0.20,-0.50,0));
			hatsokerek = Henger(0.8, 0.25, Vector3D(+0.20,+0.50,0));

		}

		void Rajzol() {
			glPushMatrix();
			glRotatef(90,1,0,0);
			teto.Rajzol();
			ttarto1.Rajzol();
			ttarto2.Rajzol();
			ttarto3.Rajzol();
			ttarto4.Rajzol();
			test.Rajzol();
			kemeny.Rajzol();

			motor.setRotate(0,1,0,0);
			motor.Rajzol();

			elsokerek.setRotate(90,1,0,0);
			elsokerek.setEltolas(Vector3D(-0.40,-0.30,+0.50));
			elsokerek.Rajzol();

			hatsokerek.setRotate(90,1,0,0);
			hatsokerek.setEltolas(Vector3D(0.40,-0.30,-0.50));
			hatsokerek.Rajzol();
			glPopMatrix();
		}

};

class Csirke {
	private:
	public:
		Henger jlab1, jlab2, blab1, blab2;
		Kup jlab3, blab3;
		Ellipszoid test;
		Henger nyak;
		Ellipszoid fej;
		Kup csor;
		Kup taraj0, taraj1, taraj2, taraj3;
		Kup farok;
		Csirke() {
			jlab1 = Henger(0.8, 0.10, Vector3D(0,0,0));
			jlab2 = Henger(0.8, 0.10, Vector3D(0,0,0));
			jlab3 = Kup(0.2, 0.25, Vector3D(0,0,0));
			blab1 = Henger(0.8, 0.10, Vector3D(0,0,0));
			blab2 = Henger(0.8, 0.10, Vector3D(0,0,0));
			blab3 = Kup(0.2, 0.25, Vector3D(0,0,0));

			test = Ellipszoid(0.8, 0.5, Vector3D(0,0,0));

			nyak = Henger(0.6, 0.20, Vector3D(0.20,-0.50,0));

			fej = Ellipszoid(0.50, 0.30, Vector3D(0,0,0));

			farok = Kup(0.5,0.4,Vector3D(0,0,0));

			csor = Kup(0.5,0.15,Vector3D(0,0,0));

			taraj0 = Kup(0.3,0.10,Vector3D(0,0,0));
			taraj1 = Kup(0.3,0.10,Vector3D(0,0,0));
			taraj2 = Kup(0.3,0.10,Vector3D(0,0,0));
			taraj3 = Kup(0.3,0.10,Vector3D(0,0,0));

		}

		void Rajzol() {

			jlab1.setRotate(-50,1,0,0);
			jlab1.setEltolas(Vector3D(0.25,-0.0,-0.40));
			jlab1.Rajzol();

			jlab2.setRotate(50,1,0,0);
			jlab2.setEltolas(Vector3D(0.25,-0.0,-0.90));
			jlab2.Rajzol();

			jlab3.setRotate(90,1,0,0);
			jlab3.setEltolas(Vector3D(0.25,-0.30,-1.2));
			jlab3.Rajzol();

			blab1.setRotate(-50,1,0,0);
			blab1.setEltolas(Vector3D(-0.25,-0.0,-0.40));
			blab1.Rajzol();

			blab2.setRotate(50,1,0,0);
			blab2.setEltolas(Vector3D(-0.25,-0.0,-0.90));
			blab2.Rajzol();

			blab3.setRotate(90,1,0,0);
			blab3.setEltolas(Vector3D(-0.25,-0.30,-1.2));
			blab3.Rajzol();

			test.Rajzol();

			nyak.setRotate(-45, 1, 0, 0);
			nyak.setEltolas(Vector3D(0,-0.35,-0.2));
			nyak.Rajzol();

			fej.setEltolas(Vector3D(0,-0.95,0.50));
			fej.Rajzol();

			farok.setRotate(200,1,0,0);
			farok.setEltolas(Vector3D(0,0.7,0.1));
			farok.Rajzol();

			csor.setRotate(200,1,0,0);
			csor.setEltolas(Vector3D(0,-1.4,0.40));
			csor.Rajzol();

			taraj0.setRotate(80,1,0,0);
			taraj0.setEltolas(Vector3D(0,-0.75,0.90));
			taraj0.Rajzol();

			taraj1.setRotate(60,1,0,0);
			taraj1.setEltolas(Vector3D(0,-0.65,0.80));
			taraj1.Rajzol();

			taraj2.setRotate(40,1,0,0);
			taraj2.setEltolas(Vector3D(0,-0.55,0.70));
			taraj2.Rajzol();

			taraj3.setRotate(20,1,0,0);
			taraj3.setEltolas(Vector3D(0,-0.50,0.60));
			taraj3.Rajzol();

		}
};

Teglatest teglatest(0.30,0.45,0.5, Vector3D(-0.5,-0.5,-0.5));
Henger henger;
Kup kup;
Ellipszoid ellipszoid;

Uthenger csirkenyomda;
Csirke csirke;

Camera camera;
Sun sun;

long time;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	time = glutGet(GLUT_ELAPSED_TIME);
	camera.Init();
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {

	glViewport(0,0,600,600);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt(30*sin(fok),30*cos(fok),60,0,0,0,0,0,1);

    gluLookAt(90*sin(fok),30,-10,0,0,0,0,0,1);

    glLightfv(GL_LIGHT0,GL_POSITION,AMBIENS_FENY);
    glEnable(GL_LIGHT0);

    float nap[4] = {sun.getPozicio().X(), sun.getPozicio().Y(), sun.getPozicio().Z(), sun.getH()};
    glLightfv(GL_LIGHT1,GL_POSITION,nap);

    glEnable(GL_LIGHT1);

    //glPushMatrix();
    //teglatest.Rajzol();
    //henger.Rajzol();
    //kup.Rajzol();
    //ellipszoid.Rajzol();
    //csirkenyomda.Rajzol();
    csirke.Rajzol();
    //glPopMatrix();

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
     // long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
	long new_time=glutGet(GLUT_ELAPSED_TIME);
		float dt=(float)(new_time - time)/(float)1000;
		if(dt>0)
		{
			fok+=0.41888*dt;
			time=new_time;
			onDisplay();
		}
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

