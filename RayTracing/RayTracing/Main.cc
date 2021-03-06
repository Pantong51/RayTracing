#include <iostream>
#include <fstream>
#include <string>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <stdlib.h>
#include <thread>
#include <future>
#include "moving_sphere.h"
#include "bvh.h"
#include "stb_image.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#define STB_IMAGE_IMPLEMENTATION




float getRandomFloat()
{
	float a = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;
	return a*b;
}

vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.0001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3(0.f, 0.f, 0.f);
	}
}
hitable *cornell_box_smoke()
{
	hitable **list = new hitable*[9];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *aMetal = new metal(vec3(0.3f, 0.3f, 1.0f), 0.1f);
	material *aGlass = new metal(vec3(0.4f, 0.4f, 1.0f), 0.1f);
	material *light = new diffuse_light(new constant_texture(vec3(4, 4, 4)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), aMetal), 15), vec3(265, 0, 295));
	list[i++] = new sphere(vec3(380, 80, 110), 80, new dielectric(1.1f));
	return new bvh_node(list, i, 0.0f, 1.0f);
}
hitable *cornell_box()
{
	hitable **list = new hitable*[9];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *aMetal = new metal(vec3(0.1f,0.1f,0.3f), 0.1f);
	material *aGlass = new metal(vec3(0.4f, 0.4f, 0.7f), 0.1f);
	material *light = new diffuse_light(new constant_texture(vec3(4, 4, 4)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), aMetal), 15), vec3(265, 0, 295));
	list[i++] = new sphere(vec3(380, 80, 110), 80, new dielectric(1.1f));
	return new bvh_node(list, i, 0.0f, 1.0f);
}

hitable *simple_light()
{
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec3(0.f, 0.f, 1.f))));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4.f, 4.f, 4.f))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4.f, 4.f, 4.f))));
	return new bvh_node(list, 4, 0.0f, 1.0f);

}

hitable *two_Spheres()
{
	texture *checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.2f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
	texture *pertext = new noise_texture(1);
	int n = 50;
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new bvh_node(list, 2, 0, 1);
}

hitable *random_scene()
{
	int n = 50000;
	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	hitable **list = new hitable *[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = (float)rand() / RAND_MAX;
			vec3 center(a + 0.9f*(float)rand() / RAND_MAX, 0.2f, b + 0.9f*(float)rand() / RAND_MAX);
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8) // diffuse
				{
					list[i++] = new sphere(center, 0.2f, new lambertian(new constant_texture(vec3(getRandomFloat(), getRandomFloat(), getRandomFloat()))));
				}
				else if (choose_mat < 0.95) // metal
				{
					list[i++] = new sphere(center, 0.2f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));// new metal(vec3((0.5f * 1.f + (float)rand() / RAND_MAX), (0.5f * 1.f + (float)rand() / RAND_MAX), (0.5f * 1.f + (float)rand() / RAND_MAX)), 0.0f));
				}
				else // glass
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0.f, 1.f, 0.f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.f, 1.f, 0.f), 1.0f, new lambertian(new constant_texture(vec3(0.4f, 0.2f, 0.1f))));
	list[i++] = new sphere(vec3(4.f, 1.f, 0.f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));
	return new bvh_node(list, i, 0.0f, 1.0f);
	//return new hitable_list(list, i);
}

struct renderdata
{
	renderdata() {}
	renderdata(float a, float b, float c, float astart, float bstart, float cstart, camera &d, hitable *e, int f, float g, float h) { X = a; Y = b; S = c; StartX = astart; StartY = bstart; StartS = cstart; cam = &d; world = e; ThreadName = f; MaxX = g; MaxY = h; }
	float X;
	float Y;
	float S;
	float StartX;
	float StartY;
	float StartS;
	int ThreadName;
	float MaxX;
	float MaxY;
	camera *cam;
	hitable *world;
	std::string tofile;
};

std::string render(renderdata *d)
{
	//(float)(((Y - j) - 1) / (Y - MaxY)) * 100)
	renderdata data;
	data = *d;
	int Y = data.Y;
	int MaxY = data.StartY;
	for (int j = data.Y - 1; j >= data.StartY; j--)
	{
		printf("Thread %i: Progress%: %f\n", data.ThreadName,((float)((Y-j)-1)/(Y- MaxY))*100 );
		for (int i = data.StartX; i < data.X; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = data.StartS; s < data.S; s++)
			{
				float u = float(i + (double)rand() / RAND_MAX) / float(data.MaxX);
				float v = float(j + (double)rand() / RAND_MAX) / float(data.MaxY);
				ray r = data.cam->get_ray(u, v);

				vec3 p = r.point_at_parameter(2.0);
				col += color(r, data.world, 0);
			}

			col /= float(data.S);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			data.tofile += std::to_string(ir);
			data.tofile += " ";
			data.tofile += std::to_string(ig);
			data.tofile += " ";
			data.tofile += std::to_string(ib);
			data.tofile += "\n";
		}
	}
	return data.tofile;
}

int main()
{
	int NumberOfX = 1125;//2250
	int NumberOfY = 1125;//1125
	int NumberOfS = 1000;
	std::ofstream out("out.ppm");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
	std::cout << "P3\n" << NumberOfX << " " << NumberOfY << "\n255\n";
	vec3 lookfrom(278.f, 278.f, -800.f);
	vec3 lookat(278.f, 278.f, 0.f);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(NumberOfX/NumberOfY), aperture, dist_to_focus, 0.0, 1.0);

	//hitable *world = random_scene();
	//hitable *world = two_Spheres();
	//hitable *world = simple_light();
	//hitable *world = cornell_box();
	hitable *world = cornell_box_smoke();
	int t = 1;
	int num = 15;
	renderdata *ts [15];
	for (t; t <= num; t++)
	{
		ts[t - 1] = new renderdata(NumberOfX, (NumberOfY / num)*t, NumberOfS, 0, ((NumberOfY / num)*t) - (NumberOfY / num), 0, cam, world, t, NumberOfX, NumberOfY);
		printf("X: %i %i %i\n", t - 1, (NumberOfX / num)*t, ((NumberOfX / num)*t) - (NumberOfX / num));
		printf("Y: %i %i %i\n", t - 1, (NumberOfY / num)*t, ((NumberOfY / num)*t) - (NumberOfY / num));
	}

	auto t0 = std::async(render, ts[0]);
	auto t1 = std::async(render, ts[1]);
	auto t2 = std::async(render, ts[2]);
	auto t3 = std::async(render, ts[3]);
	auto t4 = std::async(render, ts[4]);
	auto t5 = std::async(render, ts[5]);
	auto t6 = std::async(render, ts[6]);
	auto t7 = std::async(render, ts[7]);
	auto t8 = std::async(render, ts[8]);
	auto t9 = std::async(render, ts[9]);
	auto t10 = std::async(render, ts[10]);
	auto t11 = std::async(render, ts[11]);
	auto t12 = std::async(render, ts[12]);
	auto t13 = std::async(render, ts[13]);
	auto t14 = std::async(render, ts[14]);
	std::cout << t14.get();
	printf("t14 returned\n");
	std::cout << t13.get();
	printf("t13 returned\n");
	std::cout << t12.get();
	printf("t12 returned\n");
	std::cout << t11.get();
	printf("t11 returned\n");
	std::cout << t10.get();
	printf("t10 returned\n");
	std::cout << t9.get();
	printf("t9 returned\n");
	std::cout << t8.get();
	printf("t8 returned\n");
	std::cout << t7.get();
	printf("t7 returned\n");
	std::cout << t6.get();
	printf("t6 returned\n");
	std::cout << t5.get();
	printf("t5 returned\n");
	std::cout << t4.get();
	printf("t4 returned\n");
	std::cout << t3.get();
	printf("t3 returned\n");
	std::cout << t2.get();
	printf("t2 returned\n");
	std::cout << t1.get();
	printf("t1 returned\n");
	std::cout << t0.get();
	printf("t0 returned\n");

	//system("PAUSE");
}