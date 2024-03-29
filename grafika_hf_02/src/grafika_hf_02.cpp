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
// #define DEBUG

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

const float pi = 3.14159265358979323846;

float Szog(Vector2D v1, Vector2D v2) {
	float skalarszorzat = v1*v2;
	double cosa = skalarszorzat/(v1.Length()*v2.Length());
	return acos(cosa);
}

Vector2D Pixel2Vector(int x, int y) {

	int x_egyseg = int(glutGet(GLUT_WINDOW_WIDTH)/2);
	int y_egyseg = int(glutGet(GLUT_WINDOW_HEIGHT)/2);

	float vx = 1.0/x_egyseg * (x - x_egyseg);
	float vy = 1.0/y_egyseg * -1 * (y - y_egyseg);

	#if defined(DEBUG)
		cout << "A koordinata vektorkent: " << vx << "," << vy << endl;
	#endif

	return Vector2D(vx, vy);
}

const int BEZIER_VEZERLOPONTOK_SZAMA = 6;
const int BVSZ = BEZIER_VEZERLOPONTOK_SZAMA;
const int SZEGMENSEK_SZAMA = 10000;
const int SZSZ = SZEGMENSEK_SZAMA;

const int CATMULLROM_VEZERLOPONTOK_SZAMA = 13;
const int CRVSZ = CATMULLROM_VEZERLOPONTOK_SZAMA;
const long int CATMULLROM_SZEGMENSEK_SZAMA = 92222;
const long int CRSZSZ = CATMULLROM_SZEGMENSEK_SZAMA;

class Szin {
	private:
		float r, g, b;
	public:
		Szin() {}
		Szin(float _r, float _g, float _b) {
			this->r = _r;
			this->g = _g;
			this->b = _b;
		}

		float R() { return this->r; }
		float G() { return this->g; }
		float B() { return this->b; }
};

class BezierGorbe {
	private:
		Vector2D vezerlopontok[BVSZ];
		Vector2D szegmensek[SZSZ];
		Vector2D forgatott_szegmensek[SZSZ];
		int mvpsz;
		Szin korvonal_szin;
		Vector2D eltolas;
		Vector2D v_vektor;

		bool forgatas;

		float B(int i, float t) {
			float choose = 1;
			for (int j = 1; j <= i; j++) {
				choose *= (float)(BVSZ-i+j)/j;
			}
			return choose * pow(t, i) * pow(1-t, BVSZ-i);
		}

	public:

		BezierGorbe() {
			forgatas = false;
		}

		BezierGorbe(Szin& _korvonal_szin) {
			this->mvpsz = 0;
			this->korvonal_szin = _korvonal_szin;
			this->eltolas = Vector2D(0.0, 0.0);
			forgatas = false;
		}

		BezierGorbe(Szin& _korvonal_szin, Vector2D& _eltolas) {
			this->mvpsz = 0;
			this->korvonal_szin = _korvonal_szin;
			this->eltolas = _eltolas;
			forgatas = false;
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

		void getSzegmensek() {
			int j = 0;
			for (float i = 0; i <= 1; i = i + 0.0001) {
				szegmensek[j] = Vector2D(r(i).X(), r(i).Y());
				j++;
			}
		}

		void setVvektor(Vector2D v) {
			v_vektor = v;
		}

		Vector2D szeg(int i) {
			Vector2D result;
			if ((i >= 0) && (i < SZSZ)) { result = this->szegmensek[i]; }
			else {
				if (i < 0) { result = szegmensek[SZSZ+i]; }
				if (i >= SZSZ) { result = szegmensek[i-SZSZ]; }
			}
			return result;
		}

		void setEltolas(Vector2D& _eltolas) {
			eltolas = _eltolas;
		}

		void setForgatas(bool _forgatas) {
			forgatas = _forgatas;
		}

		void Forgatas() {
			Vector2D y_tengely = Vector2D(0,1.0);
			double szog = Szog(y_tengely, v_vektor);

			int j = 0;
			for (float i = 0; i <= 1; i = i + 0.0001) {
				float x = szeg(i).X() * cos(szog) - szeg(i).Y() * sin(szog);
				float y = szeg(i).X() * sin(szog) + szeg(i).Y() * cos(szog);
				forgatott_szegmensek[j] = Vector2D(x, y);
				j++;
			}

		}

		void Rajzol(float scale_x = 1.0, float scale_y = 1.0) {

			getSzegmensek();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslatef(eltolas.X(), eltolas.Y(), 0.f);
			if (forgatas) {	Forgatas(); }
			glScalef(scale_x, scale_y, 0.0f);

			#if defined(DEBUG)
				/* A kontrollpontok kirajzolasa: */
				glColor3f(0.5f,1.0f,0.56f);
				glPointSize(5.0f);
				glBegin(GL_POINTS);
					for (int i = 0; i <= BVSZ; i++) {
						cout << this->vezerlopontok[i].X() << "," << this->vezerlopontok[i].Y() << endl;
						glVertex2f(this->vezerlopontok[i].X(), this->vezerlopontok[i].Y());
					}
				glEnd();
			#endif

			glColor3f(korvonal_szin.R(),korvonal_szin.G(),korvonal_szin.B());
			glPointSize(2.5f);
			glBegin(GL_POINTS);
				for (long int i = 0; i < SZSZ; i++) {
					if (forgatas) {
						glVertex2f(forgatott_szegmensek[i].X(), forgatott_szegmensek[i].Y());
					}
					else {
						glVertex2f(szegmensek[i].X(), szegmensek[i].Y());
					}

				}
			glEnd();

			glLoadIdentity();

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

		Szin gorbeszin;
		Szin vpszin;

		bool vprajzolasa;

		Vector2D szegmensek[CRSZSZ];
		Vector2D forgatott_szegmensek[CRSZSZ];

		Vector2D eltolas;

		Vector2D v_vektor;

		bool forgatas;

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

		CatmullRomGorbe() {
			eltolas = Vector2D(0.0,0.0);
			forgatas = false;
		}

		CatmullRomGorbe(Vector2D& _eltolas) {
			eltolas = _eltolas;
			forgatas = false;
		}

		void setVprajzolasa(bool _vprajzolasa) {
			vprajzolasa = _vprajzolasa;
		}

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
			float result;
			if ((i >= 0) && (i < CRVSZ)) { result = ta[i]; }
			else {
				if (i < 0) { result = ta[CRVSZ+i]; }
				if (i >= CRVSZ) { result = ta[i-CRVSZ]; }
			}
			return result;
		}

		void LoadV() {
			for (int i = 0; i < CRVSZ; i++) {
				v[i] = ((fi(i)-fi(i-1))/(ti(i)-ti(i-1)) + (fi(i+1)-fi(i))/(ti(i+1)-ti(i))) * 0.5;
			}
		}

		Vector2D vi(int i) { return v[i]; }

		void Init(Vector2D v[13], Szin _gorbeszin, Szin _vpszin) {

			setVprajzolasa(false);

			gorbeszin = _gorbeszin;
			vpszin = _vpszin;

			for (int i = 0; i < 13; i++) {
				vp[i] = v[i];
			}


		}

		void setEltolas(Vector2D& _eltolas) {
			eltolas = _eltolas;
		}

		void setForgatas(bool _forgatas) {
			forgatas = _forgatas;
		}

		void getSzegmensek() {
			long int j = 0;
			for (int i = 0; i < CRVSZ; i++) {
				for (float t = ti(i); t < ti(i+1); t = t + 0.00001) {
					szegmensek[j] = Vector2D(r(t,i).X(), r(t,i).Y());
					j++;
				}
			}
		}

		Vector2D sz(long int i) {
			Vector2D result;
			if ((i >= 0) && (i < CRSZSZ)) { result = this->szegmensek[i]; }
			else {
				if (i < 0) { result = szegmensek[CRSZSZ+i]; }
				if (i >= CRVSZ) { result = szegmensek[i-CRSZSZ]; }
			}
			return result;
		}

		float getGorbehossz() {
			Vector2D szegmens;
			float hossz = 0;
			for (long int i = 0; i < CRSZSZ; i++) {
				szegmens = (sz(i) - sz(i+1));
				hossz = hossz + szegmens.Length();
			}
			return hossz;
		}

		Vector2D setScale() {
			float hossz = getGorbehossz();
			float arany = hossz/100*4;
			return Vector2D(arany, arany);
		}

		void setVvektor(Vector2D v) {
			v_vektor = v;
		}

		void Forgatas() {
			Vector2D y_tengely = Vector2D(0,1.0);
			double szog = Szog(y_tengely, v_vektor);

			for (long int i = 0; i < CRSZSZ; i++) {
				float x = sz(i).X() * cos(szog) - sz(i).Y() * sin(szog);
				float y = sz(i).X() * sin(szog) + sz(i).Y() * cos(szog);
				forgatott_szegmensek[i] = Vector2D(x, y);
			}

		}

		void Rajzol(float scale_x = 1.0, float scale_y = 1.0) {

			LoadTa();
			LoadV();
			LoadA();
			LoadB();

			getSzegmensek();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(eltolas.X(), eltolas.Y(), 0.f);
			if (forgatas) {	Forgatas(); }
			glScalef(scale_x, scale_y, 0.0f);

			glColor3f(gorbeszin.R(),gorbeszin.G(),gorbeszin.B());
			glPointSize(2.5f);
			glBegin(GL_POINTS);
			long int j = 0;
			for (int i = 0; i < CRVSZ; i++) {
				for (float t = ti(i); t < ti(i+1); t = t + 0.00001) {
					if (forgatas) {
						glVertex2f(forgatott_szegmensek[j].X(), forgatott_szegmensek[j].Y());
					}
					else {
						glVertex2f(szegmensek[j].X(), szegmensek[j].Y());
					}
					j++;
				}
			}
			glEnd();

			if (vprajzolasa) {
				/* A kontrollpontok kirajzolasa: */
				glColor3f(vpszin.R(),vpszin.G(),vpszin.B());
				glPointSize(10.0f);
				glBegin(GL_POINTS);
				for (int i = 0; i < CRVSZ; i++) {
					glVertex2f(this->vp[i].X(), this->vp[i].Y());
				}
				glEnd();
			}

			glLoadIdentity();

		}

		void VezerloPontPakolo(int x, int y) {
			Vector2D kp = Pixel2Vector(x, y);

			float max_tavolsag;
			float tavolsag;
			int pont;

			max_tavolsag = sqrt(pow(fabs(vp[0].X() - kp.X()),2) + pow(fabs(vp[0].Y() - kp.Y()),2));
			pont = 0;

			for (int i = 0; i < 13; i++) {
				tavolsag = sqrt(pow(fabs(vp[i].X() - kp.X()),2) + pow(fabs(vp[i].Y() - kp.Y()),2));
				if (max_tavolsag >= tavolsag) {
					max_tavolsag = tavolsag;
					pont = i;
				}
			}

			if ((pont == 12) || (pont == 0)) {
				vp[0] = vp[12] = kp;
			}
			else {
				vp[pont] = kp;
			}

		}

};

const int HAZPONT = 9;

class Poligon {
	private:
		int aktualispszam;
		Vector2D eltolas;

	public:
		Vector2D p[HAZPONT*8];
		Poligon() {
			aktualispszam = HAZPONT;
			eltolas = Vector2D(0.0,0.0);
		}

		Poligon(Vector2D& _eltolas) {
			aktualispszam = HAZPONT;
			eltolas = _eltolas;
		}

		void Rajzol(float scale_x = 1.0, float scale_y = 1.0) {
			glColor3f(0.20f,0.20f,0.20f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(eltolas.X(), eltolas.Y(), 0.f);
			glScalef(scale_x, scale_y, 0.0f);


			#if defined(DEBUG)
				/* A kontrollpontok kirajzolasa: */
				glColor3f(1.0f,0.78f,0.56f);
				glPointSize(5.0f);
				glBegin(GL_POINTS);
					for (int i = 0; i < aktualispszam; i++) {
						//cout << p[i].X() << "," << p[i].Y() << endl;
						glVertex2f(p[i].X(), p[i].Y());
					}
				glEnd();
			#endif

			glColor3f(0.5f,1.0f,0.56f);
			glBegin(GL_LINES);
			for (int i = 0; i < aktualispszam-1; i++) {
				glVertex2f(p[i].X(), p[i].Y());
				glVertex2f(p[i+1].X(), p[i+1].Y());
			}
			glEnd();

			glLoadIdentity();
		}

		Vector2D r(int i) {
			Vector2D result;
			if ((i >= 0) && (i < aktualispszam)) { result = p[i]; }
			else {
				if (i < 0) { result = p[aktualispszam+i]; }
				if (i >= aktualispszam) { result = p[i-aktualispszam]; }
			}
			return result;
		}

		Vector2D getFelezoPont(Vector2D v1, Vector2D v2) {
			return Vector2D((v1.X()+v2.X())/2,(v1.Y()+v2.Y())/2);
		}

		void setEltolas(Vector2D& _eltolas) {
			eltolas = _eltolas;
		}

		void CatmullClark(int szint) {
			if (szint < 4) {
				for (int s = 0; s < szint; s++) {
					Vector2D temp[aktualispszam*2];
					int j = 0;
					for (int i = 0; i < aktualispszam; i++) {
						temp[j] = p[i];
						j++;
						temp[j] = getFelezoPont(r(i),r(i+1));
						j++;
					}
					aktualispszam = aktualispszam*2;

					for (int i = 0; i < aktualispszam; i++) {
						p[i] = temp[i];
					}

					for (int i = 0; i < aktualispszam; i = i + 2) {
						p[i] = r(i)*0.5 + r(i-1)*0.25 + r(i+1)*0.25;
					}
				}
			}


		}


};

Szin Black(0.0,0.0,0.0);

class Csiga {
	private:
		CatmullRomGorbe test;
		Poligon haz1, haz2;
		BezierGorbe szem1, szem2;

		Vector2D szem1_eltolas, szem2_eltolas;
		Vector2D eltolas;

		Vector2D v_vektor;
	public:
		Csiga() {
			eltolas = Vector2D(0.0, 0.0);
			szem1 = BezierGorbe(Black,eltolas);
			szem2 = BezierGorbe(Black,eltolas);

			Vector2D testvp[13] = { Vector2D(+0.00,+0.50),
									Vector2D(-0.075,+0.525),
									Vector2D(-0.35,+0.85),
									Vector2D(-0.20,+0.50),
									Vector2D(-0.15,+0.20),
									Vector2D(-0.30,-0.35),
									Vector2D(+0.00,-0.95),
									Vector2D(+0.30,-0.35),
									Vector2D(+0.15,+0.20),
									Vector2D(+0.20,+0.50),
									Vector2D(+0.35,+0.85),
									Vector2D(+0.075,+0.525),
									Vector2D(+0.00,+0.50) };

			Szin testszin(0.33333, 0.41961, 0.18431);
			Szin testvpszin(0,0,0);

			test.Init(testvp, testszin, testvpszin);
			test.setEltolas(eltolas);
			test.setForgatas(true);

			szem1.VezerlopontHozzaadasa(Vector2D(-0.35, +1.00));
			szem1.VezerlopontHozzaadasa(Vector2D(-0.50, +1.00));
			szem1.VezerlopontHozzaadasa(Vector2D(-0.50, +0.45));
			szem1.VezerlopontHozzaadasa(Vector2D(-0.20, +0.45));
			szem1.VezerlopontHozzaadasa(Vector2D(-0.20, +1.00));
			szem1.VezerlopontHozzaadasa(Vector2D(-0.35, +1.00));

			szem2.VezerlopontHozzaadasa(Vector2D(+0.35, +1.00));
			szem2.VezerlopontHozzaadasa(Vector2D(+0.20, +1.00));
			szem2.VezerlopontHozzaadasa(Vector2D(+0.20, +0.45));
			szem2.VezerlopontHozzaadasa(Vector2D(+0.50, +0.45));
			szem2.VezerlopontHozzaadasa(Vector2D(+0.50, +1.00));
			szem2.VezerlopontHozzaadasa(Vector2D(+0.35, +1.00));

			szem1.setEltolas(eltolas);
			szem2.setEltolas(eltolas);
			szem1.setForgatas(false);
			szem2.setForgatas(false);

			haz1.p = { Vector2D(+0.00,+0.00),

					   Vector2D(-0.15,+0.10),
					   Vector2D(-0.15,-0.10),
					   Vector2D(-0.25,-0.35),
					   Vector2D(+0.00,-0.75),
					   Vector2D(+0.25,-0.35),
					   Vector2D(+0.15,-0.10),
					   Vector2D(+0.15,+0.10),

					   Vector2D(+0.00,+0.00) };

			haz2.p = { Vector2D(-0.00,-0.10),

					   Vector2D(-0.05,+0.00),
					   Vector2D(-0.05,-0.10),
					   Vector2D(-0.15,-0.35),
					   Vector2D(+0.00,-0.55),
					   Vector2D(+0.15,-0.35),
					   Vector2D(+0.05,-0.10),
					   Vector2D(+0.05,+0.00),

					   Vector2D(+0.00,-0.10) };

			haz1.CatmullClark(3);

			haz2.CatmullClark(3);

			haz1.setEltolas(eltolas);
			haz2.setEltolas(eltolas);

		}

		void setEltolas(Vector2D& _eltolas) {
			eltolas = _eltolas;
			test.setEltolas(eltolas);
			haz1.setEltolas(eltolas);
			haz2.setEltolas(eltolas);
 			szem1.setEltolas(eltolas);
			szem2.setEltolas(eltolas);
		}

		Vector2D getEltolas() {
			return eltolas;
		}

		void setVvektor(Vector2D v) {
			v_vektor = v;
		}

		void Rajzol(float scale_x = 1.0, float scale_y = 1.0) {
			szem1.setVvektor(v_vektor);
		    szem1.Rajzol(scale_x, scale_y);
		    szem1.setVvektor(v_vektor);
		    szem2.Rajzol(scale_x, scale_y);
		    test.setVvektor(v_vektor);
		    test.Rajzol(scale_x, scale_y);
		    haz1.Rajzol(scale_x, scale_y);
		    haz2.Rajzol(scale_x, scale_y);
		}

};

class Palya {
	private:
		CatmullRomGorbe palyavonal;
	public:
		Palya() {
			Vector2D palyavonalvp[13] = { Vector2D(+0.34,+0.68),
									 	  Vector2D(-0.05,+0.69),
									 	  Vector2D(-0.09,+0.19),
										  Vector2D(-0.54,+0.40),
										  Vector2D(-0.81,+0.18),
										  Vector2D(-0.82,-0.29),
										  Vector2D(-0.25,-0.42),
										  Vector2D(+0.19,-0.83),
										  Vector2D(+0.37,-0.28),
										  Vector2D(+0.65,-0.31),
										  Vector2D(+0.80,-0.10),
										  Vector2D(+0.84,+0.51),
										  Vector2D(+0.34,+0.68) };

			Szin palyavonalszin(0.23, 0.56, 0.80);

			Szin palyavonalvpszin(0.93, 0.90, 0.00);

			palyavonal.Init(palyavonalvp, palyavonalszin, palyavonalvpszin);
			palyavonal.setVprajzolasa(true);
		}

		void VezerloPontPakolo(int x, int y) {
			palyavonal.VezerloPontPakolo(x, y);
		}

		Vector2D setScale() {
			return palyavonal.setScale();
		}

		Vector2D getPalyapont(long int i) {
			return palyavonal.sz(i);
		}

		float getPalyahossz() {
			return palyavonal.getGorbehossz();
		}

		Vector2D r(long int i) {
			return palyavonal.sz(i);
		}

		void Rajzol() {
			palyavonal.Rajzol();
		}
};

Csiga csiga;
Palya palya;

bool zoom = false;

void Animalas(float utolso_rajzolas, float time) {
	float eltelt_ido = time - utolso_rajzolas;
	Vector2D r = palya.r(eltelt_ido*(long int)(CRSZSZ/5000));
	Vector2D v = r/eltelt_ido;

	csiga.setVvektor(v);
	csiga.setEltolas(r);
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.55294f, 0.71373f, 0.00f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    // ...

    if (zoom) {
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluOrtho2D(csiga.getEltolas().X()-0.5, csiga.getEltolas().X()+0.5, csiga.getEltolas().Y()-0.5, csiga.getEltolas().Y()+0.5);
    }
    else {
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluOrtho2D(-1,1,-1,1);
    }

    palya.Rajzol();
    csiga.Rajzol(palya.setScale().X(), palya.setScale().Y());

    glutSwapBuffers();     				// Buffercsere: rajzolas vege
}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {

	#if defined(DEBUG)
	PrintPressedKey(key, x, y);
	PrintTime();
	#endif

	if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
	if (key == 'z') {
		zoom = !zoom;
	}

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN) {  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
    	palya.VezerloPontPakolo(x, y);
    	glutPostRedisplay( ) ;
    }
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek

float time = 0;

void onIdle( ) {
	float utolso_rajzolas = time;

	long time = glutGet(GLUT_ELAPSED_TIME) % 5000;		// program inditasa ota eltelt ido

	Animalas(utolso_rajzolas, time);

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

