#ifndef game_misc_CIAN
#define game_misc_CIAN
	template <class T> class duo {
		public:
			T x, y;
			duo<T>() { x=(T) 0; y=(T) 0; };
			duo<T>(T a, T b) { x=a; y=b; };
			template <class U> duo<U> operator-(U a) { return duo<U>(x-a, y-a); };
			template <class U> duo<U> operator/(U a) { return duo<U>(x/a, y/a); };
			template <class U> duo<U> operator-(duo<U> a) { return duo<U>(x-a.x, y-a.y); };
			template <class U> duo<U> operator/(duo<U> a) { return duo<U>(x/a.x, y/a.y); };
	};

	float px2a(int px, int size);
	duo<float> px2a(duo<int> px, duo<int> size);

#endif
