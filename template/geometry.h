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

			inline Point operator-(const Point& b) const {
				return Point(x - b.x , y - b.y);
			}

			inline Point operator+(const Point& b) const {
				return Point(x + b.x , y + b.y);
			}

			inline Point operator*(const Point& b) const {
				return Point(x * b.x , y * b.y);
			}

			inline void operator-=(const Point& b) {
				x -= b.x; y -= b.y;
			}

			inline void operator+=(const Point& b) {
				x += b.x; y += b.y;
			}

			inline void operator*=(const Point& b) {
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
}

using namespace geometry;
// }}}

