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

const float KAMERASEBESSEG = 0.6;

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

class Color {
	private:
		float r, g, b, a;
	public:
		Color() {}
		Color(float _r, float _g, float _b, float _a) {
			this->r = _r;
			this->g = _g;
			this->b = _b;
			this->a = _a;
		}

		Color operator+(const Color &c) {
			return Color(this->r+c.r, this->g+c.g, this->b+c.b, this->a+c.a);
		}

		void operator+=(const Color &c) {
			this->r+=c.r;
			this->g+=c.g;
			this->b+=c.b;
			this->a+=c.a;
		}

		Color operator+(const float c) {
			return Color(this->r+c, this->g+c, this->b+c, this->a+c);
		}

		void operator+=(const float c) {
			this->r+=c;
			this->g+=c;
			this->b+=c;
			this->a+=c;
		}

		Color operator-(const Color &c) {
			return Color(this->r-c.r, this->g-c.g, this->b-c.b, this->a-c.a);
		}

		void operator-=(const Color &c) {
			this->r-=c.r;
			this->g-=c.g;
			this->b-=c.b;
			this->a-=c.a;
		}

		Color operator-(const float c) {
			return Color(this->r-c, this->g-c, this->b-c, this->a-c);
		}

		void operator-=(const float c) {
			this->r-=c;
			this->g-=c;
			this->b-=c;
			this->a-=c;
		}

		Color operator*(const Color &c) {
			return Color(this->r*c.r, this->g*c.g, this->b*c.b, this->a*c.a);
		}

		void operator*=(const Color &c) {
			this->r*=c.r;
			this->g*=c.g;
			this->b*=c.b;
			this->a*=c.a;
		}

		Color operator*(const float c) {
			return Color(this->r*c, this->g*c, this->b*c, this->a*c);
		}

		void operator*=(const float c) {
			this->r*=c;
			this->g*=c;
			this->b*=c;
			this->a*=c;
		}

		Color operator/(const Color &c) {
			return Color(this->r/c.r, this->g/c.g, this->b/c.b, this->a/c.a);
		}

		void operator/=(const Color &c) {
			this->r/=c.r;
			this->g/=c.g;
			this->b/=c.b;
			this->a/=c.a;
		}

		Color operator/(const float c) {
			return Color(this->r/c, this->g/c, this->b/c, this->a/c);
		}

		void operator/=(const float c) {
			this->r/=c;
			this->g/=c;
			this->b/=c;
			this->a/=c;
		}

		float R() { return this->r; }
		float G() { return this->g; }
		float B() { return this->b; }
		float A() { return this->a; }
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

			float nap_intenzitas_ambiens[4] = {0.0, 0.0, 0.0, 0.0};
			float nap_intenzitas_diffuz[4] = {1.0, 1.0, 1.0, 1.0};
			float nap_intenzitas_spekularis[4] = {2.0, 2.0, 2.0, 1.0};
			float ambiens_feny_ambiens[4] = {0.1, 0.2, 0.3, 1.0};
			float ambiens_feny_diffuz[4] = {0.0, 0.0, 0.0, 0.0};
			float ambiens_feny_spekularis[4] = {0.0, 0.0, 0.0, 0.0};

			glLightfv(GL_LIGHT1,GL_AMBIENT,nap_intenzitas_ambiens);
			glLightfv(GL_LIGHT1,GL_DIFFUSE,nap_intenzitas_diffuz);
			glLightfv(GL_LIGHT1,GL_SPECULAR,nap_intenzitas_spekularis);

			glLightfv(GL_LIGHT0,GL_AMBIENT,ambiens_feny_ambiens);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,ambiens_feny_diffuz);
			glLightfv(GL_LIGHT0,GL_SPECULAR,ambiens_feny_spekularis);

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
		float feny[4];
	public:
		Sun() {
			pozicio = Vector3D(-1,1,1);
			h = 0;
			feny[0] = 1, feny[1] = 0, feny[2] = 0, feny[3] = 1;
		}

		Vector3D getPozicio() {
			return this->pozicio;
		}

		float getH() {
			return this->h;
		}

		float getFenyR() {
			return feny[0];
		}

		float getFenyG() {
			return feny[1];
		}

		float getFenyB() {
			return feny[2];
		}

		float getFenyH() {
			return feny[3];
		}

		void setFeny(long t) {
			float tc = sin(t*PI/10000);
			feny[1] = tc;
			feny[2] = tc;
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
			glBegin(GL_TRIANGLES);
				glNormal3f(normalvektor.X(),normalvektor.Y(),normalvektor.Z());
				glTexCoord2d(0,0);
				glVertex3f(csucspont[0].X(),csucspont[0].Y(),csucspont[0].Z());
				glTexCoord2d(1,1);
				glVertex3f(csucspont[1].X(),csucspont[1].Y(),csucspont[1].Z());
				glTexCoord2d(0,1);
				glVertex3f(csucspont[2].X(),csucspont[2].Y(),csucspont[2].Z());
			glEnd();
		}
};

typedef float szintomb[4];
struct szin { szintomb t; };

class Material {
	private:
		Color ambient;
		Color diffuse;
		Color specular;
		float shininess;

	public:
		Material() {}
		Material(Color _ambient, Color _diffuse, Color _specular, float _shininess) {
			this->ambient = _ambient;
			this->diffuse = _diffuse;
			this->specular = _specular;
			this->shininess = _shininess;
		}

		szin getAmbient() {
			szin sz;
			sz.t[0] = ambient.R();
			sz.t[1] = ambient.G();
			sz.t[2] = ambient.B();
			sz.t[3] = ambient.A();
			return sz;
		}

		szin getDiffuse() {
			szin sz;
			sz.t[0] = diffuse.R();
			sz.t[1] = diffuse.G();
			sz.t[2] = diffuse.B();
			sz.t[3] = diffuse.A();
			return sz;
		}

		szin getSpecular() {
			szin sz;
			sz.t[0] = specular.R();
			sz.t[1] = specular.G();
			sz.t[2] = specular.B();
			sz.t[3] = specular.A();
			return sz;
		}

		float getShininess() {
			return shininess;
		}
};

class Texture {
public:
	unsigned char bitmap[16*16*3];
	Texture(int R1, int G1, int B1, int R2, int G2, int B2) {
		long time = glutGet(GLUT_ELAPSED_TIME);
		srand(time);

		for (int i = 0; i < 256; i++) {
			bitmap[i*3] = R1 + rand() % R2;
			bitmap[i*3+1] = G1 + rand() % G2;
			bitmap[i*3+2] = B1 + rand() % B2;
		}
	}
};

const int TEXTURAKSZAMA = 3;
unsigned int texturak[TEXTURAKSZAMA];

const int FU = 0;
Texture fu = Texture(0,192,0,128,64,32);
const int CSH = 1;
Texture hengerlo = Texture(128,128,128,64,64,64);
const int ASZFALT = 2;
Texture aszfalt = Texture(32,32,32,10,10,10);

Color ezustambiens = Color(0.19225, 0.19225, 0.19225, 1.0);
Color ezustdiffuz = Color(0.50754, 0.50754, 0.50754, 1.0);
Color ezustspekularis = Color(0.508273, 0.508273, 0.508273, 1.0);
float ezustfenyesseg = 51.2;
Material ezust = Material(ezustambiens, ezustdiffuz, ezustspekularis, ezustfenyesseg);

Color pezustambiens = Color(0.23125, 0.23125, 0.23125, 1.0);
Color pezustdiffuz = Color(0.2775, 0.2775, 0.2775, 1.0);
Color pezustspekularis = Color(0.773911, 0.773911, 0.773911, 1.0);
float pezustfenyesseg = 89.6;
Material pezust = Material(pezustambiens, pezustdiffuz, pezustspekularis, pezustfenyesseg);

Color bronzambiens = Color(0.2125, 0.1275, 0.054, 1.0);
Color bronzdiffuz = Color(0.714, 0.4284, 0.18144, 1.0);
Color bronzspekularis = Color(0.393548, 0.271906, 0.166721, 1.0);
float bronzfennyesseg = 25.6;
Material bronz = Material(bronzambiens, bronzdiffuz, bronzspekularis, pezustfenyesseg);

Color pirosambiens = Color(0.25, 0.25, 0.25, 1.0);
Color pirosdiffuz = Color(1.0, 0.2, 0.0, 1.0);
Color pirosspekularis = Color(0.0, 0.0, 0.0, 0.0);
float pirosfenyesseg = 0.0;
Material piros = Material(pirosambiens, pirosdiffuz, pirosspekularis, pirosfenyesseg);

Color sargaambiens = Color(0.25, 0.25, 0.25, 1.0);
Color sargadiffuz = Color(1.0, 1.0, 0.0, 1.0);
Color sargaspekularis = Color(0.0, 0.0, 0.0, 0.0);
float sargafenyesseg = 0.0;
Material sarga = Material(sargaambiens, sargadiffuz, sargaspekularis, sargafenyesseg);

class Ojjektum {
	private:
		float forgatas_szog;
		float forgatas_x;
		float forgatas_y;
		float forgatas_z;
		Vector3D eltolas;

		Material anyag;
		int textura;

	protected:
		Vertex vertexek[2048];
		int vertexszam;

	public:

		Ojjektum() {
			vertexszam = 0;
			eltolas = Vector3D(0,0,0);
			textura = -1;
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

		void setAnyag(Material _anyag) {
			this->anyag = _anyag;
		}

		void setTextura(int _textura) {
			this->textura = _textura;
		}

		void Rajzol() {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, anyag.getDiffuse().t);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, anyag.getSpecular().t);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, anyag.getAmbient().t);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, anyag.getShininess());

			if (textura > -1) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,texturak[textura]);
			}

			glPushMatrix();
			glTranslatef(eltolas.X(),eltolas.Y(),eltolas.Z());
			glRotatef(forgatas_szog,forgatas_x,forgatas_y,forgatas_z);
			for (int i = 0; i < vertexszam; i++) {
				vertexek[i].Rajzol();
			}
			glPopMatrix();


			if (textura > -1) {
				glDisable(GL_TEXTURE_2D);
			}
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
		Vector3D sebesseg;
		Vector3D gyorsulas;
		Vector3D r;
	public:
		Teglatest teto, ttarto1, ttarto2, ttarto3, ttarto4, kemeny, test, motor, tengely1, tengely2;
		Henger elsokerek, hatsokerek;

		Uthenger() {

			gyorsulas = Vector3D(0,0,0);

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

		Vector3D getR() {
			return this->r;
		}

		Vector3D getSebesseg() {
			return this->sebesseg;
		}

		void setR(Vector3D v) {
			this->r = v;
		}

		void setR(long t) {
			this->sebesseg += this->gyorsulas*t;
			this->r +=  this->sebesseg*t;
		}

		Vector3D getGyorsulas() {
			return this->gyorsulas;
		}

		void setGyorsulasF() {
			this->gyorsulas = Vector3D(0.000001,0,0);
		}

		void setGyorsulasB() {
			this->gyorsulas = Vector3D(-0.000001,0,0);
		}

		void setGyorsulasN() {
			this->gyorsulas = Vector3D(0,0,0);
		}

		void Rajzol() {
			glPushMatrix();
			glTranslatef(r.X(),r.Y(),r.Z());
			glRotatef(90,1,0,0);

			teto.setAnyag(pezust);
			teto.Rajzol();

			ttarto1.setAnyag(ezust);
			ttarto1.Rajzol();
			ttarto2.setAnyag(ezust);
			ttarto2.Rajzol();
			ttarto3.setAnyag(ezust);
			ttarto3.Rajzol();
			ttarto4.setAnyag(ezust);
			ttarto4.Rajzol();

			test.setAnyag(ezust);
			test.Rajzol();

			kemeny.setAnyag(ezust);
			kemeny.Rajzol();

			motor.setRotate(0,1,0,0);
			motor.setAnyag(ezust);
			motor.Rajzol();

			elsokerek.setRotate(90,1,0,0);
			elsokerek.setEltolas(Vector3D(-0.40,-0.30,+0.50));
			elsokerek.setAnyag(bronz);
			elsokerek.setTextura(CSH);
			elsokerek.Rajzol();

			hatsokerek.setRotate(90,1,0,0);
			hatsokerek.setEltolas(Vector3D(0.40,-0.30,-0.50));
			hatsokerek.setAnyag(bronz);
			hatsokerek.setTextura(CSH);
			hatsokerek.Rajzol();

			glPopMatrix();
		}
};

class Csirke {
	private:
		Vector3D helyzet;
		Vector3D headpoz;
		Vector3D sebesseg;
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

			headpoz = Vector3D(0,-0.95,0.50);

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

		Vector3D getHelyzet() {
			return this->helyzet;
		}

		void setHelyzet(Vector3D v) {
			this->helyzet = v;
		}

		void setHelyzet(long dt) {
			this->helyzet += this->sebesseg*dt;
		}

		Vector3D getHeadpoz() {
			return this->headpoz;
		}

		Vector3D getSebesseg() {
			return this->sebesseg;
		}

		void setSebesseg(Vector3D v) {
			this->sebesseg = v;
		}



		void Rajzol() {

			glPushMatrix();
			//glTranslatef(1.0,1.0,0.53);
			glTranslatef(helyzet.X(), helyzet.Y(), helyzet.Z());
			glScalef(0.4,0.4,0.4);

			jlab1.setRotate(-50,1,0,0);
			jlab1.setEltolas(Vector3D(0.25,-0.0,-0.40));
			jlab1.setAnyag(piros);
			jlab1.Rajzol();

			jlab2.setRotate(50,1,0,0);
			jlab2.setEltolas(Vector3D(0.25,-0.0,-0.90));
			jlab2.setAnyag(piros);
			jlab2.Rajzol();

			jlab3.setRotate(90,1,0,0);
			jlab3.setEltolas(Vector3D(0.25,-0.30,-1.2));
			jlab3.setAnyag(piros);
			jlab3.Rajzol();

			blab1.setRotate(-50,1,0,0);
			blab1.setEltolas(Vector3D(-0.25,-0.0,-0.40));
			blab1.setAnyag(piros);
			blab1.Rajzol();

			blab2.setRotate(50,1,0,0);
			blab2.setEltolas(Vector3D(-0.25,-0.0,-0.90));
			blab2.setAnyag(piros);
			blab2.Rajzol();

			blab3.setRotate(90,1,0,0);
			blab3.setEltolas(Vector3D(-0.25,-0.30,-1.2));
			blab3.setAnyag(piros);
			blab3.Rajzol();

			test.setAnyag(sarga);
			test.Rajzol();

			nyak.setRotate(-45, 1, 0, 0);
			nyak.setEltolas(Vector3D(0,-0.35,-0.2));
			nyak.setAnyag(sarga);
			nyak.Rajzol();

			fej.setEltolas(headpoz);
			fej.setAnyag(sarga);
			fej.Rajzol();

			farok.setRotate(200,1,0,0);
			farok.setEltolas(Vector3D(0,0.7,0.1));
			farok.setAnyag(sarga);
			farok.Rajzol();

			csor.setRotate(200,1,0,0);
			csor.setEltolas(Vector3D(0,-1.4,0.40));
			csor.setAnyag(piros);
			csor.Rajzol();

			taraj0.setRotate(80,1,0,0);
			taraj0.setEltolas(Vector3D(0,-0.75,0.90));
			taraj0.setAnyag(piros);
			taraj0.Rajzol();

			taraj1.setRotate(60,1,0,0);
			taraj1.setEltolas(Vector3D(0,-0.65,0.80));
			taraj1.setAnyag(piros);
			taraj1.Rajzol();

			taraj2.setRotate(40,1,0,0);
			taraj2.setEltolas(Vector3D(0,-0.55,0.70));
			taraj2.setAnyag(piros);
			taraj2.Rajzol();

			taraj3.setRotate(20,1,0,0);
			taraj3.setEltolas(Vector3D(0,-0.50,0.60));
			taraj3.setAnyag(piros);
			taraj3.Rajzol();

			glPopMatrix();
		}
};

Teglatest mezo(50.00,50.00,0.01, Vector3D(0,0,-0.002));
Teglatest ut(50.00,1.00,0.01, Vector3D(0,0,-0.001));

Vector3D csirkenyomda_start = Vector3D(0.0,0.0,0.55);
Uthenger csirkenyomda;

Csirke csibe;
Csirke csiba;
Csirke csipa;

Vector3D csirkekozpont = Vector3D(1.5,1.0,0.53);

Vector3D kovcsirkefej;

Camera camera;
Sun sun;

long time;
float fok = 0;

float sotetoldal[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{-sun.getPozicio().X()/sun.getPozicio().Z(),-sun.getPozicio().Y()/sun.getPozicio().Z(),0,0},
		{0,0,0.005,1}
};

float CoordX = 0;
float CoordY = 0;
float CoordZ = 0;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	time = glutGet(GLUT_ELAPSED_TIME);
	camera.Init();

	glGenTextures(TEXTURAKSZAMA,texturak);
	glBindTexture(GL_TEXTURE_2D,texturak[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,fu.bitmap);
	glBindTexture(GL_TEXTURE_2D,texturak[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,hengerlo.bitmap);
	glBindTexture(GL_TEXTURE_2D,texturak[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,aszfalt.bitmap);

	mezo.setTextura(FU);
	mezo.setAnyag(sarga);
	ut.setTextura(ASZFALT);
	ut.setAnyag(sarga);

	csibe.setHelyzet(csirkekozpont-Vector3D(0.5,0.0,0.0));
	csiba.setHelyzet(csirkekozpont);
	csipa.setHelyzet(csirkekozpont+Vector3D(0.5,0.0,0.0));

	kovcsirkefej = csibe.getHeadpoz()+csibe.getHelyzet();

	csirkenyomda.setR(csirkenyomda_start);
}

void onDisplayAfter(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt(30*sin(fok),30*cos(fok),60,0,0,0,0,0,1);
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

    //gluLookAt(60,0,0,0,0,0,0,0,1);

    glLightfv(GL_LIGHT0,GL_POSITION,AMBIENS_FENY);
    glEnable(GL_LIGHT0);

    float nap[4] = {sun.getPozicio().X(), sun.getPozicio().Y(), sun.getPozicio().Z(), sun.getH()};
    glLightfv(GL_LIGHT1,GL_POSITION,nap);

    float napocska[4] = {sun.getFenyR(), sun.getFenyG(), sun.getFenyB(), sun.getFenyH()};
    glLightfv(GL_LIGHT1,GL_DIFFUSE,napocska);

    glEnable(GL_LIGHT1);

    mezo.Rajzol();
    ut.Rajzol();
    csirkenyomda.Rajzol();
    csibe.Rajzol();
    csiba.Rajzol();
    csipa.Rajzol();

    glMultMatrixf(&sotetoldal[0][0]);
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    csibe.Rajzol();
    csiba.Rajzol();
    csipa.Rajzol();
    csirkenyomda.Rajzol();
    glEnable(GL_LIGHTING);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    glViewport(0,0,300,300);
    onDisplayAfter(30*sin(fok),30*cos(fok),60,csirkekozpont.X(),csirkekozpont.Y(),csirkekozpont.Z(),0,0,1);
    glViewport(0,300,300,300);
    onDisplayAfter(kovcsirkefej.X(), kovcsirkefej.Y(), kovcsirkefej.Z()+CoordZ, csirkekozpont.X()+CoordX,csirkekozpont.Y()+CoordY,csirkekozpont.Z(),0,0,1);
    glViewport(300,150,300,300);
    onDisplayAfter(30*sin(fok),30*cos(fok),60,0,0,0,0,0,1);

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

	#if defined(DEBUG)
	PrintPressedKey(key, x, y);
	PrintTime();
	#endif

    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

    if (key == 'f') {
    	csirkenyomda.setGyorsulasF();
    	glutPostRedisplay();
    }

    if (key == 'b') {
		csirkenyomda.setGyorsulasB();
		glutPostRedisplay();
	}

    if (key == 'n') {
		csirkenyomda.setGyorsulasN();
		cout << csirkenyomda.getGyorsulas().X() << endl;
		glutPostRedisplay();
	}

    if (key == 'c') {
    	csibe.setSebesseg(Vector3D(0.0,-0.001,0));
    	glutPostRedisplay();
    }

    if (key == 'j') {
        	CoordX = CoordX + 0.01;
        	glutPostRedisplay();
    }
    if (key == 'l') {
    		CoordX = CoordX - 0.01;
    		glutPostRedisplay();
    }
    if (key == 'i') {
			CoordY = CoordY + 0.01;
			glutPostRedisplay();
	}
	if (key == 'k') {
			CoordY = CoordY - 0.01;
			glutPostRedisplay();
	}
    if (key == '8') {
			CoordZ = CoordZ + 0.01;
			glutPostRedisplay();
	}
	if (key == '9') {
			CoordZ = CoordZ - 0.01;
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
	long aktualisido = glutGet(GLUT_ELAPSED_TIME);

	long suntime = aktualisido % 10000;
	sun.setFeny(suntime);

	long dt = aktualisido - time;
	csirkenyomda.setR(dt);
	csibe.setHelyzet(dt);

	float elteltido = (float)(aktualisido - time)/1000;



	if (elteltido > 0) {
		fok+=KAMERASEBESSEG*elteltido;
		time = aktualisido;
		glutPostRedisplay();
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

