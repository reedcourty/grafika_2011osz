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
#endif

const int WIDTH = 600;
const int HEIGHT = 600;

const int OBJECTSNUM = 1;

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
		float r, g, b;
	public:
		Color() {}
		Color(float _r, float _g, float _b) {
			this->r = _r;
			this->g = _g;
			this->b = _b;
		}

		Color operator+(const Color &c) {
			return Color(this->r+c.r, this->g+c.g, this->b+c.b);
		}

		void operator+=(const Color &c) {
			this->r+=c.r;
			this->g+=c.g;
			this->b+=c.b;
		}

		Color operator+(const float c) {
			return Color(this->r+c, this->g+c, this->b+c);
		}

		void operator+=(const float c) {
			this->r+=c;
			this->g+=c;
			this->b+=c;
		}

		Color operator-(const Color &c) {
			return Color(this->r-c.r, this->g-c.g, this->b-c.b);
		}

		void operator-=(const Color &c) {
			this->r-=c.r;
			this->g-=c.g;
			this->b-=c.b;
		}

		Color operator-(const float c) {
			return Color(this->r-c, this->g-c, this->b-c);
		}

		void operator-=(const float c) {
			this->r-=c;
			this->g-=c;
			this->b-=c;
		}

		Color operator*(const Color &c) {
			return Color(this->r*c.r, this->g*c.g, this->b*c.b);
		}

		void operator*=(const Color &c) {
			this->r*=c.r;
			this->g*=c.g;
			this->b*=c.b;
		}

		Color operator*(const float c) {
			return Color(this->r*c, this->g*c, this->b*c);
		}

		void operator*=(const float c) {
			this->r*=c;
			this->g*=c;
			this->b*=c;
		}

		Color operator/(const Color &c) {
			return Color(this->r/c.r, this->g/c.g, this->b/c.b);
		}

		void operator/=(const Color &c) {
			this->r/=c.r;
			this->g/=c.g;
			this->b/=c.b;
		}

		Color operator/(const float c) {
			return Color(this->r/c, this->g/c, this->b/c);
		}

		void operator/=(const float c) {
			this->r/=c;
			this->g/=c;
			this->b/=c;
		}

		float R() { return this->r; }
		float G() { return this->g; }
		float B() { return this->b; }
};

class Bitmap {
	public:
		Color pixels[WIDTH][HEIGHT];

		Bitmap() {
			for (int x=0; x<WIDTH; x++) {
				for (int y=0; y<HEIGHT; y++) {
					pixels[x][y] = Color(1.0,1.0,1.0);
				}
			}
		}

		void PutBitmap() {
			glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);
		}

};

class Ray {
	private:
		Vector3D kezdo_pont;
		Vector3D irany_vektor;
	public:
		Ray() {
			kezdo_pont = Vector3D(0,0,0);
			irany_vektor = Vector3D(0,0,0);
		}

		Vector3D getKezdo_pont() { return this->kezdo_pont; }
		Vector3D getIrany_vektor() { return this->irany_vektor; }

		void setKezdo_pont(Vector3D v) { this->kezdo_pont = v; }
		void setIrany_vektor(Vector3D v) {
			this->irany_vektor = v;
			//this->irany_vektor.Normalize();
		}

		float& Px() { return this->kezdo_pont.X(); }
		float& Py() { return this->kezdo_pont.Y(); }
		float& Pz() { return this->kezdo_pont.Z(); }

		float& Vx() { return this->irany_vektor.X(); }
		float& Vy() { return this->irany_vektor.Y(); }
		float& Vz() { return this->irany_vektor.Z(); }

};

class Light {
	private:
		Color szin;
		Vector3D pont;
	public:
		Light();
};

class Camera {
	private:
		Vector3D pont;
	public:
		Camera() {this->pont = Vector3D(0,0,-500);}

	Vector3D getPont() { return this->pont; }
};

class Object {
	protected:
		Color szin;
	public:
		virtual double Intersect(Ray &r) = 0;

		Color getSzin() { return this->szin; }
};

class TheOneRing : public Object {
	private:
		double radius;
		double vastagsag;
		double magassag;
		Vector3D kozeppont;

	public:
		TheOneRing(double _radios, double _vastagsag, double _magassag, Vector3D _kozeppont) {
			this->radius = _radios;
			this->vastagsag = _vastagsag;
			this->magassag = _magassag;
			this->kozeppont = _kozeppont;
			this->szin = Color(1.00, 0.00, 0.00);
		}

		double Intersect(Ray &r) {

			double a = pow(r.Vx(),2) + pow(r.Vy(),2);
			double b = 2*r.Px()*r.Vx()+2*r.Py()*r.Vy();
			double c = pow(r.Px(),2) + pow(r.Py(),2) - pow(radius,2);

			double d = pow(b,2) - 4*a*c;

			/*
			double t1 = (-b + sqrt(pow(b,2)-4*a*c)) / (2*a);
			double t2 = (-b - sqrt(pow(b,2)-4*a*c)) / (2*a);

			Vector3D p = Vector3D(r.getKezdo_pont() + r.getIrany_vektor()*t1);
			//cout << r.getIrany_vektor().Y() << " ";
			if ((p.Y()>magassag) && (p.Y()<0)) {
				d = -1;
			}
			*/

			return d;
		}
};

class Sphere : public Object {
	public:
		Vector3D kozeppont;
		double radius;

		Sphere(Vector3D _kozeppont = Vector3D(0,0,0), double _radius = 1) {
			this->kozeppont = _kozeppont;
			this->radius = _radius;
			this->szin = Color(0.0,0.0,1.0);
		}

		double Intersect(Ray &r) {
			double dx = r.Vx();
			double dy = r.Vy();
			double dz = r.Vz();

			#if defined(DEBUG)
    			cout << "Intersect - dx: " << dx << ", dy: " << dy << ", dz: " << dz << endl;
			#endif

			double x0 = r.Px();
			double y0 = r.Py();
			double z0 = r.Pz();

			double cx = kozeppont.X();
			double cy = kozeppont.Y();
			double cz = kozeppont.Z();

			double R = radius;

			double a = dx * dx + dy * dy + dz * dz;
			double b = 2 * dx * (x0 - cx) + 2 * dy * (y0 - cy) + 2 * dz * (z0 - cz);
			double c = cx * cx + cy * cy + cz * cz + x0 * x0 + y0 * y0 + z0 * z0 - 2 * (cx * x0 + cy * y0 + cz * z0) - R * R;

			double d = b * b - 4 * a * c;

			#if defined(DEBUG)
    			cout << "Intersect - a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << endl;
			#endif

			if(d < 0) {
				return -1.0;
			}

			double t = ((-1.0 * b - sqrt(d)) / (2.0 * a));

			#if defined(DEBUG)
    			cout << "Intersect - t: " << t << endl;
			#endif

			if (t > 0.00005) {
				return t;
			}
			else {
				return 0.0;
			}
		}

		Vector3D getNormal(Vector3D& intersect) {
			Vector3D v = intersect - kozeppont;
			v.Normalize();
			return v;
		}
};


class Scene {
	private:
		Object* objects[OBJECTSNUM];

		int obj_szam;
	public:
		Scene() { obj_szam = 0; }

		Camera cam;

		void addObject(Object* obj) {
			this->objects[obj_szam] = obj;
			this->obj_szam++;
		}

		Color Trace(Ray& r) {
			double t;
			Object* o;
			Color c = Color(0,0,0);
			for (int obj = 0; obj<OBJECTSNUM; obj++) {
				o = this->objects[obj];
				t = o->Intersect(r);

				#if defined(DEBUG)
    				cout << "Trace - t: " << t << endl;
				#endif

				if (t > 0) {
					c = o->getSzin();
				}
				else {

				}
			}

			//#if defined(DEBUG)
    		//	cout << "Trace - c: " << c.R() << "," << c.G() << "," << c.B() << endl;
			//#endif

			return c;
		}


};

TheOneRing tor(50, 50, 50, Vector3D(0.5, 0.5, 0.5));
Sphere gomb;
Scene sc;

Bitmap bm;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	sc.addObject(&gomb);
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

    for (int i = 0; i < WIDTH; i++) {
    	for (int j = 0; j < HEIGHT; j++) {

    		Ray ray;
    		Vector3D kezdo_pont = Vector3D((i - WIDTH/2), (j - HEIGHT/2), 0);

    		//#if defined(DEBUG)
    		//	cout << "onDisplay() - kezdo_pont: (" << kezdo_pont.X() << "," << kezdo_pont.Y() << "," << kezdo_pont.Z() << ")" << endl;
			//#endif

    		ray.setKezdo_pont(kezdo_pont);

    		Vector3D irany_vektor = ray.getKezdo_pont() - sc.cam.getPont();

    		//#if defined(DEBUG)
    		//	cout << "onDisplay() - irany_vektor: (" << ray.getIrany_vektor().X() << "," << ray.getIrany_vektor().Y() << "," << ray.getIrany_vektor().Z() << ")" << endl;
			//#endif

    		ray.setIrany_vektor(irany_vektor);

			//#if defined(DEBUG)
    		//	cout << "onDisplay() - irany_vektor set utan: (" << ray.getIrany_vektor().X() << "," << ray.getIrany_vektor().Y() << "," << ray.getIrany_vektor().Z() << ")" << endl;
			//#endif

    		bm.pixels[i][j] = sc.Trace(ray);
    	}
    }


    bm.PutBitmap();

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN) {};  // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
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

