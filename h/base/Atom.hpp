#ifndef H_BASE_ATOM
#define H_BASE_ATOM

namespace base {

	template <typename T>
	class Atom {

	public:

		const T dimension;
		const T rows;
		const T columns;
		T** matrix;

		Atom(const T dimension, const T rows, const T columns, T** matrix) :
		dimension(dimension), rows(rows), columns(columns), matrix(matrix) {}

	};

}

#endif // H_BASE_ATOM
