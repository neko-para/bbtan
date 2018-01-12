#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <list>
using std::list;

struct Vec {
	double x, y;
	Vec() : x(1), y(0) {}
	Vec(double _x, double _y) : x(_x), y(_y) {}
	void Norm() {
		double l = sqrt(x * x + y * y);
		x /= l;
		y /= l;
	}
	double length2() const {
		return x * x + y * y;
	}
	Vec Rotate(double a) const {
		double s = sin(a), c = cos(a);
		return Vec(x * c - y * s, x * s + y * c);
	}
	void Rev(const Vec& n);
	Vec operator-() const {
		return Vec(-x, -y);
	}
};

Vec operator+(const Vec& a, const Vec& b) {
	return Vec(a.x + b.x, a.y + b.y);
}

Vec operator-(const Vec& a, const Vec& b) {
	return Vec(a.x - b.x, a.y - b.y);
}

Vec operator*(const Vec& a, double b) {
	return Vec(a.x * b, a.y * b);
}

double operator*(const Vec& a, const Vec& b) {
	return a.x * b.x + a.y * b.y;
}

double operator^(const Vec& a, const Vec& b) {
	return a.x * b.y - a.y * b.x;
}

void Vec::Rev(const Vec& n) {
	*this = *this + ((n * (*this * n) - *this) * 2);
}

struct Block {
	Vec p;
	double r;
	int cnt;
	Block(double x, double y, double r, double cnt) : p(x, y),  r(r), cnt(cnt) {}
};

struct Ball {
	Vec p, v;
	double r;
	Ball(double x, double y, double vx, double vy, double r) : p(x, y), v(vx, vy), r(r) {}
	bool Test(Block& b) {
		Vec dlt = b.p - p;
		double sr = b.r + r;
		if (dlt.length2() <= sr * sr) {
			--b.cnt;
			dlt.Norm();
			v.Rev(dlt);
			v = -v;
		}
		if (b.cnt == 0) {
			return true;
		} else {
			return false;
		}
	}
};

const double PI = acos(-1);
list<Block> blocks;
list<Ball> balls;

void Circle(const Vec& v, double r) {
	glBegin(GL_POLYGON);
	Vec f(r, 0);
	for (double i = 0; i < PI * 2; i += 0.1) {
		Vec vc = v + f.Rotate(i);
		glVertex2d(vc.x, vc.y);
	}
	glEnd();
}

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(400, 600, "bbtan", 0, 0);
	glfwMakeContextCurrent(window);
	glLoadIdentity();
	glOrtho(0, 400, 0, 600, -1, 1);
	glClearColor(0, 0, 0, 1);
	blocks.push_back(Block(20, 20, 20, 10));
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3d(1, 1, 1);
		for (Block& b : blocks) {
			Circle(b.p, b.r);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
