// [NAME]: Basic 2D geometry helpers
// [PURPOSE]: Provides integer Point struct with vector operations plus orientation, collinearity, and segment intersection tests.
// Typical use: quick geometry primitives for segment overlap/intersection checks.
// [COMPLEXITY]:
//   - orientation/intersect: O(1)
//   - memory: O(1) per object
// [USAGE]:
//   - Use geometry::Point(x,y); functions like pointLocation(a,b,c) return orientation of c relative to segment ab.
//   - intersect(p1,p2,p3,p4) returns whether closed segments intersect; assumes int coordinates and inclusive endpoints.
// {{{ geometry
namespace geometry {
	typedef int T;
	class Point {
		public:
			T x, y;

			Point() {
				this->x = 0;
				this->y = 0;
			}

			Point(T x, T y) {
				this->x = x;
				this->y = y;
			}

			void read() {
				cin>>x>>y;
			}

			Point operator-(const Point& b) const {
				return Point(x - b.x , y - b.y);
			}

			Point operator+(const Point& b) const {
				return Point(x + b.x , y + b.y);
			}

			Point operator*(const Point& b) const {
				return Point(x * b.x , y * b.y);
			}

			void operator-=(const Point& b) {
				x -= b.x; y -= b.y;
			}

			void operator+=(const Point& b) {
				x += b.x; y += b.y;
			}

			void operator*=(const Point& b) {
				x *= b.x; y *= b.y;
			}

	};

	T crossProduct(const Point& a, const Point& b) {
		return a.x*b.y - a.y*b.x;
	}

	int pointLocation(Point& a, Point b, Point c) { // check the location of point c acording to segment a->b
		b-=a; c-=a;
		T cp = crossProduct(b,c);
		if(cp == 0)
			return 0; // TOUCH
		else if(cp > 0)
			return 1; // LEFT
		else 
			return -1; // RIGHT
	}
	
	bool collinear(Point p1, Point p2, Point p3, Point p4) {
		bool ans = 1;
		for(int i=0;i<2;i++) {
			int loc1 = pointLocation(p1,p2,p3); 
			int loc2 = pointLocation(p1,p2,p3); 
			ans &= (loc1 == 0 && loc2 == 0);
			swap(p1,p3);
			swap(p2,p4);
		}
		return ans;
	}

	bool intersect(Point p1, Point p2, Point p3, Point p4) {
		Point p[] = {p1,p2,p3,p4};
		for(int i=0;i<2;i++) {
			int loc1 = pointLocation(p[0],p[1],p[2]); 
			int loc2 = pointLocation(p[0],p[1],p[3]); 
			if((loc1 > 0 && loc2 > 0) || (loc1 < 0 && loc2 < 0)) {
				return 0;
			}
			swap(p[0],p[2]);
			swap(p[1],p[3]);
		}
		if(collinear(p[0],p[1],p[2],p[3])) {
			for(int _=0;_<2;_++) {
				if((max(p[0].x,p[1].x) < min(p[2].x,p[3].x)) || (max(p[0].y,p[1].y) < min(p[2].y,p[3].y))) {
					return 0;
				}
				swap(p[0],p[2]);
				swap(p[1],p[3]);
			}
		}
		return 1;
	}
}

using namespace geometry;
// }}}
