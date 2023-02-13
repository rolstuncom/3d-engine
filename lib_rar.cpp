#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace sf;
using namespace std;

bool f = 0;

//////////////////settings//////////////////
float fps = 100;

//////////////////system variables//////////////////
Vector2f sizew = Vector2f(1000, 1000);
RenderWindow window(VideoMode(sizew.x, sizew.y), "game", Style::Default);
View view(Vector2f(0.f, 0.f), Vector2f(sizew.x, sizew.y));
////////////////// simulation variables //////////////////
Vector2f objsize = Vector2f(4, 4);
RectangleShape point;
//////////////////////////////////////////////////
class Vector4f
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	Vector4f operator+(Vector4f op2);
	Vector4f operator-(Vector4f op2);
	Vector4f operator*(Vector4f op2);
	Vector4f operator/(Vector4f op2);
	void operator=(Vector4f op2);

	Vector4f();
	Vector4f(float xx, float yy, float zz, float ww);


};

Vector4f::Vector4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Vector4f::Vector4f(float xx, float yy, float zz, float ww)
{
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

Vector4f Vector4f::operator+(Vector4f op2)
{
	Vector4f temp;
	temp.x = x + op2.x;
	temp.y = y + op2.y;
	temp.z = z + op2.z;
	temp.w = w + op2.w;
	return temp;
}
Vector4f Vector4f::operator-(Vector4f op2)
{
	Vector4f temp;
	temp.x = x - op2.x;
	temp.y = y - op2.y;
	temp.z = z - op2.z;
	temp.w = w - op2.w;
	return temp;
}
Vector4f Vector4f::operator*(Vector4f op2)
{
	Vector4f temp;
	temp.x = x * op2.x;
	temp.y = y * op2.y;
	temp.z = z * op2.z;
	temp.w = w * op2.w;
	return temp;
}
Vector4f Vector4f::operator/(Vector4f op2)
{
	Vector4f temp;
	temp.x = x / op2.x;
	temp.y = y / op2.y;
	temp.z = z / op2.z;
	temp.w = w / op2.w;
	return temp;
}
void Vector4f::operator=(Vector4f op2)
{
	x = op2.x;
	y = op2.y;
	z = op2.z;
	w = op2.w;
}
///////////////////////////////////////////////////////////////

bool out(Vector2f pos, Vector2f tpos, Vector2f ramka)
{
	if (pos.x < tpos.x + ramka.x && pos.x > tpos.x - ramka.x && pos.y < tpos.y + ramka.y && pos.y > tpos.y - ramka.y)return true;
	return false;
}


class Object
{
public:
	vector <Vector3f> source_vertex_table;
	vector <Vector3f> vertex_table;
	vector <Vector4f> quad_table;
	vector <Vector3f> normal_table;

	Vector3f position = Vector3f(0, 0, 0);
	Vector3f rotation = Vector3f(0, 0, 0);
	Vector3f scale = Vector3f(1, 1, 1);

	void setpos(Vector3f pos);
	void resize(Vector3f res);
	void rotate(Vector3f rot);
	void reset();

	void setvertexpos(int n, Vector3f pos);
	void setquad(int n, Vector4f link);

	void generate_normal();

	Vector3f get_center();

	void operator=(Object op2);

	Object();
	Object(int vertex_amount);
};

Object::Object()
{
	source_vertex_table.resize(1);
	vertex_table.resize(1);
	quad_table.resize(1);
}
Object::Object(int vertex_amount)
{
	source_vertex_table.resize(vertex_amount);
	vertex_table.resize(vertex_amount);
	quad_table.resize((vertex_amount * 4));
}

void Object::setpos(Vector3f pos)
{
	position = pos;
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i] = source_vertex_table[i] + pos;
	}
}
void Object::resize(Vector3f res)
{
	scale += res;
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i] -= position;
	}
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i].x *= res.x;
		vertex_table[i].y *= res.y;
		vertex_table[i].z *= res.z;

		source_vertex_table[i].x *= res.x;
		source_vertex_table[i].y *= res.y;
		source_vertex_table[i].z *= res.z;


	}
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i] += position;
	}
}
void Object::rotate(Vector3f rot)
{
	rot.x /= 114.591582866;
	rot.y /= 114.591582866;
	rot.z /= 114.591582866;
	rotation = rot;
	vector <Vector3f> temp = source_vertex_table;
	for (int i = 0; vertex_table.size() > i; i++)
	{

		if (rotation.x != 0)
		{
			vertex_table[i].y = temp[i].y * cos(rotation.x) + temp[i].z * -sin(rotation.x);
			vertex_table[i].z = -temp[i].y * sin(rotation.x) + temp[i].z * cos(rotation.x);
		}

		if (rotation.y != 0)
		{
			vertex_table[i].x = temp[i].x * cos(rotation.y) + temp[i].z * sin(rotation.y);
			vertex_table[i].z = -temp[i].x * sin(rotation.y) + temp[i].z * cos(rotation.y);
		}

		if (rotation.z != 0)
		{
			vertex_table[i].x = temp[i].x * cos(rotation.z) + temp[i].y * -sin(rotation.z);
			vertex_table[i].y = temp[i].x * sin(rotation.z) + temp[i].y * cos(rotation.z);
		}
	}
}
void Object::reset()
{
	rotation = Vector3f(0, 0, 0);
	for (int i = 0; source_vertex_table.size() > i; i++)
	{
		vertex_table[i] = source_vertex_table[i] + position;
	}

}

void Object::setvertexpos(int n, Vector3f pos)
{
	source_vertex_table[n] = pos;
	vertex_table[n] = pos;
}
void Object::setquad(int n, Vector4f link)
{
	quad_table[n] = link;
}

void Object::generate_normal()
{

	Vector3f center;
	for (int i = 0; i > source_vertex_table.size(); i++)
	{
		center += source_vertex_table[i];
	}
	center.x = center.x / source_vertex_table.size();
	center.y = center.y / source_vertex_table.size();
	center.z = center.z / source_vertex_table.size();



}

Vector3f Object::get_center()
{
	Vector3f center;
	for (int i = 0; i > vertex_table.size(); i++)
	{
		center.x += vertex_table[i].x;
		center.y += vertex_table[i].y;
		center.z += vertex_table[i].z;
	}
	center.x = center.x / vertex_table.size();
	center.y = center.y / vertex_table.size();
	center.z = center.z / vertex_table.size();
	return center;
}

void Object::operator=(Object op2)
{
	source_vertex_table = op2.source_vertex_table;
	vertex_table = op2.vertex_table;
	quad_table = op2.quad_table;
	normal_table = op2.normal_table;

	position = op2.position;
	rotation = op2.rotation;
	scale = op2.scale;
}
///////////////////////////////////////////////////////////////
class Camera {
public:
	float minz = 10;
	float maxz = 1000;
	float focal = 100;
	Vector3f Cam_Rot = Vector3f(0, 0, 0);
	Vector3f Cam_Pos = Vector3f(0, 0, 0);
	Camera(Vector3f cp, float foc, float mnz, float mxz);
	void Render(RenderWindow& ren, Object ob, RectangleShape point);
	//void Cam_rotate(vector<Object>ob);

};

Camera::Camera(Vector3f cp, float foc, float mnz, float mxz)
{
	minz = mnz;
	maxz = mxz;
	focal = foc;
	Cam_Pos = cp;
}

void Camera::Render(RenderWindow& ren, Object ob, RectangleShape point)
{
	sf::ConvexShape convex;
	convex.setPointCount(4);

	float precalc = 0;
	vector <Vector2f> points(ob.vertex_table.size());

	for (float i = 0; i < points.size(); i++)
	{
		if ((ob.vertex_table[i].z - Cam_Pos.z) > minz && (ob.vertex_table[i].z - Cam_Pos.z) < maxz)
		{

			precalc = focal + ob.vertex_table[i].z - Cam_Pos.z;
			points[i].x = (focal * (ob.vertex_table[i].x - Cam_Pos.x)) / precalc;
			points[i].y = (focal * (ob.vertex_table[i].y - Cam_Pos.y)) / precalc;
			point.setPosition(points[i]);
			ren.draw(point);
		}

	}
	/*for (float i = 0; i < points.size(); i++)
	{

		convex.setPoint(0, points[ob.quad_table[i].x]);
		convex.setPoint(1, points[ob.quad_table[i].y]);
		convex.setPoint(2, points[ob.quad_table[i].z]);
		convex.setPoint(3, points[ob.quad_table[i].w]);
		ren.draw(convex);
	}
	return;*/
}

void world(RenderWindow& ren, vector<Object> ob, RectangleShape point, Camera cam)
{
	for (int i = 0; i > ob.size(); i++)
	{
		cam.Render(ren, ob[i], point);
	}

}



int main()
{

	Camera cam(Vector3f(0, 0, -200), 600, 0, 1000000);
	Object ob(8);
	ob.setvertexpos(0, Vector3f(-50, -50, -50));
	ob.setvertexpos(1, Vector3f(-50, -50, 50));
	ob.setvertexpos(2, Vector3f(50, -50, 50));
	ob.setvertexpos(3, Vector3f(50, -50, -50));
	ob.setvertexpos(4, Vector3f(-50, 50, -50));
	ob.setvertexpos(5, Vector3f(-50, 50, 50));
	ob.setvertexpos(6, Vector3f(50, 50, 50));
	ob.setvertexpos(7, Vector3f(50, 50, -50));

	ob.setquad(0, Vector4f(0, 4, 7, 3));
	ob.setquad(1, Vector4f(7, 6, 2, 3));
	ob.setquad(2, Vector4f(6, 5, 1, 2));
	ob.setquad(3, Vector4f(4, 5, 1, 0));
	ob.setquad(4, Vector4f(4, 5, 6, 7));
	ob.setquad(5, Vector4f(0, 1, 2, 3));

	int gr = 0;
	window.setFramerateLimit(fps);
	////////////////////////just things//////////////////////////////////
	point.setFillColor(Color(0, 255, 0, 255));
	point.setSize(objsize);
	point.setOrigin(objsize.x / 2, objsize.y / 2);

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.setView(view);

		window.clear(Color(0, 0, 0, 255));
		f = 1;
		while (f)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				cam.Cam_Pos.y += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				cam.Cam_Pos.y -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				cam.Cam_Pos.x -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				cam.Cam_Pos.x += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				cam.Cam_Pos.z += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				cam.Cam_Pos.z -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				ob.resize(Vector3f(1.01, 1.01, 1.01));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				ob.resize(Vector3f(0.99, 0.99, 0.99));
			}

			gr++;
			ob.rotate(Vector3f(0, gr, 0));
			cam.Render(window, ob, point);

			f = 0;
		};
		window.display();
	}

	return 0;
}