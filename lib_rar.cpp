#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

bool f = 0;


//////////////////settings//////////////////
float fps = 100;
float focal = 64;
int edge_amount = 8;
//////////////////system variables//////////////////

Vector2f sizew = Vector2f(1000, 1000);
RenderWindow window(VideoMode(sizew.x, sizew.y), "game", Style::Default);
View view(Vector2f(0.f, 0.f), Vector2f(sizew.x, sizew.y));

////////////////// simulation variables //////////////////

Vector2f objsize = Vector2f(4, 4);

RectangleShape point;

Vector3f cam_pos = Vector3f(0, 0, 0);
float precalc = 0;

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
	Vector4f(float xx,float yy,float zz,float ww);
	
	
};

Vector4f::Vector4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Vector4f::Vector4f(float xx,float yy,float zz,float ww)
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


class Object
{
public:
	vector <Vector3f> source_vertex_table;
	vector <Vector3f> vertex_table;
	vector <Vector4f> quad_table;
	vector <Vector3f> normal_table;

	Vector3f position = Vector3f(0,0,0);
	Vector3f rotation = Vector3f(0,0,0);
	Vector3f scale = Vector3f(0,0,0);

	void setpos(Vector3f pos);
	void resize(Vector3f res);
	void rotate(Vector3f rot);

	void setvertexpos(int n, Vector3f pos);
	void setquad(int n, Vector4f link);

	void generate_normal();


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
	position += pos;
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i] = source_vertex_table[i] + pos;
	}
}

void Object::resize(Vector3f res)
{
	for (float i = 0; i < vertex_table.size(); i++)
	{
		vertex_table[i].x = vertex_table[i].x + res.x;
		vertex_table[i].y = vertex_table[i].y + res.y;
		vertex_table[i].z = vertex_table[i].z + res.z;
	}
}
void Object::rotate(Vector3f rot)
{
	float p = 0;
	rotation += rot;
	if (rotation.x > 360)
	{
		p = rotation.x;
		rotation.x -= 360*int(p/360);
	}
	if (rotation.y > 360)
	{
		p = rotation.y;
		rotation.y -= 360 * int(p / 360);
	}
	if (rotation.z > 360)
	{
		p = rotation.z;
		rotation.z -= 360 * int(p / 360);
	}
	
	for (int i = 0; vertex_table.size() > i; i++)
	{
		vertex_table[i].y = source_vertex_table[i].y * cos(rotation.x) + source_vertex_table[i].z * sin(rotation.x);
		vertex_table[i].z = -source_vertex_table[i].y* sin(rotation.x) + source_vertex_table[i].z * cos(rotation.x);
	}
	for (int i = 0; vertex_table.size() > i; i++)
	{
		vertex_table[i].x = source_vertex_table[i].x * cos(rotation.y) - source_vertex_table[i].z * sin(rotation.y);
		vertex_table[i].z = source_vertex_table[i].x * sin(rotation.y) + source_vertex_table[i].z * cos(rotation.y);
	}
	for (int i = 0; vertex_table.size() > i; i++)
	{
		
		vertex_table[i].x = source_vertex_table[i].x * cos(rotation.z) + source_vertex_table[i].y * sin(rotation.z);
		vertex_table[i].y = -source_vertex_table[i].x * sin(rotation.z) + source_vertex_table[i].y * cos(rotation.z);
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

class Camera {
public:
	float minz = 10;
	float maxz = 1000;
	float focal = 100;
	Vector3f Cam_Rot = Vector3f(0, 0, 0);
	Vector3f Cam_Pos = Vector3f(0, 0, 0);
	Camera(Vector3f cp, float foc, float mnz, float mxz);
	void Render(RenderWindow& ren, Object ob, RectangleShape point);
	
	
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
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10.f, 10.f)),
		sf::Vertex(sf::Vector2f(150.f, 150.f))
	};
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
	return;
}
void world(RenderWindow& ren, vector<Object> ob, RectangleShape point,Camera cam)
{
	for (int i = 0; i > ob.size(); i++)
	{
		cam.Render(ren, ob[i], point);
	}
	
}


int main()
{

	Camera cam(Vector3f(0, 0, 0), 100, 1, 1000000);
	Object ob(8);
	Object ob1(8);
	Object ob2(8);
	ob.setvertexpos(0, Vector3f(100, 100, 100));
	ob.setvertexpos(1, Vector3f(100, 100, 200));
	ob.setvertexpos(2, Vector3f(200, 100, 200));
	ob.setvertexpos(3, Vector3f(200, 100, 100));
	ob.setvertexpos(4, Vector3f(100, 200, 100));
	ob.setvertexpos(5, Vector3f(100, 200, 200));
	ob.setvertexpos(6, Vector3f(200, 200, 200));
	ob.setvertexpos(7, Vector3f(200, 200, 100));

	ob.setquad(0, Vector4f(0, 1,0,0));
	ob.setquad(1, Vector4f(0, 3,0,0));
	ob.setquad(2, Vector4f(1, 2,0,0));
	ob.setquad(3, Vector4f(3, 2,0,0));
	ob.setquad(4, Vector4f(4, 0,0,0));
	ob.setquad(5, Vector4f(5, 1,0,0));
	ob.setquad(6, Vector4f(5, 4,0,0));
	ob.setquad(7, Vector4f(6, 2,0,0));
	ob.setquad(8, Vector4f(6, 5,0,0));
	ob.setquad(9, Vector4f(7, 3,0,0));
	ob.setquad(10, Vector4f(7, 4,0,0));
	ob.setquad(11, Vector4f(7, 6,0,0));

	window.setFramerateLimit(fps);
	////////////////////////just things//////////////////////////////////
	point.setFillColor(Color(0, 255, 0, 255));
	point.setSize(objsize);
	point.setOrigin(objsize.x / 2, objsize.y / 2);
	while (window.isOpen())
	{
		//if (!texture.loadFromFile("texture/test_texture.png")) ///////for texture loading

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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				focal+=4;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				focal-=4;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				cam_pos.y += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				cam_pos.y -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				cam_pos.x -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				cam_pos.x += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				cam_pos.z += 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				cam_pos.z -= 10;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			{
				ob.rotate(Vector3f(0.0001,0.0001,0.0001));
			}
			cam.minz = 0;
			cam.focal = focal;
			cam.Cam_Pos = cam_pos;
			
			ob1 = ob;
			ob2 = ob;

			ob1.setpos(Vector3f(325, 160, 132));
			ob2.setpos(Vector3f(470, 0, 611));

			cam.Render(window, ob, point);
			cam.Render(window, ob1, point);
			cam.Render(window, ob2, point);
			f = 0;
		};
		window.display();
	}

	return 0;
}