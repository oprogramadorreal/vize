#include <boost/test/unit_test.hpp>
#include <ayla/thread_pool.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(thread_pool)

namespace {
	SizeType _factorial(SizeType n) {
		if (n == 0u) {
			return 1u;
		}

		return n*_factorial(n-1);
	}
}

class FactorialCalculator final : public ThreadPool::Task {
public:
	FactorialCalculator(SizeType n)
		: _n(n), _result(0u)
	{ }

	virtual void execute() override {
		_result = _factorial(_n);
	}

	inline SizeType getResult() { return _result; }
	inline SizeType getN() const { return _n; }

private:
	SizeType _n;
	SizeType _result;
};


BOOST_AUTO_TEST_CASE( factorialTest ) {
	BOOST_CHECK(_factorial(0) == 1);
	BOOST_CHECK(_factorial(5) == 120);
	BOOST_CHECK(_factorial(6) == 720);
	
	const SizeType numOfTasks = 12u;

	ThreadPool::TasksGroup group;
	ThreadPool pool;
	
	// create tasks group
	for (SizeType i = 0u; i < numOfTasks; ++i) {
		group.push_back(std::make_unique<FactorialCalculator>(rand() % numOfTasks));
	}

	// force the same execution some times
	for (SizeType i = 0u; i < 7u; ++i) {
		pool.execute(group);
	}

	// check results
	for (auto it = group.begin(); it != group.end(); ++it) {
		const auto task = static_cast<FactorialCalculator*>(it->get());
		BOOST_CHECK(_factorial(task->getN()) == task->getResult());
	}
}

////////////////////////////////////////////////////////////////////

#define MATRIX_ORDER 100
#define MAX_MATRIX_VALUE 100
#define MIN_MATRIX_VALUE -100

typedef std::vector<std::vector<Float> > TestMatrix;

void printM(const TestMatrix& m) {
	for (SizeType i = 0u; i < MATRIX_ORDER; ++i) {
		for (SizeType j = 0u; j < MATRIX_ORDER; ++j) {
			std::cout << m[i][j] << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
}

Float multiplyRowColumn(const TestMatrix& mA, SizeType aRow, const TestMatrix& mB, SizeType bCol) {
	Float sum = 0.0f;
	
	for (SizeType k = 0u; k < MATRIX_ORDER; ++k) {
		sum += mA[aRow][k] * mB[k][bCol];
	}
	
	return sum;
}

void multiply(const TestMatrix& mA, const TestMatrix& mB, TestMatrix& result) {
	for (SizeType i = 0u; i < MATRIX_ORDER; ++i) {
		for (SizeType j = 0u; j < MATRIX_ORDER; ++j) {		
			result[i][j] = multiplyRowColumn(mA, i, mB, j);
		}
	}
}

class ParallelMatrixMult : public ThreadPool::Task {
public:
	ParallelMatrixMult(const TestMatrix& mA, SizeType aRow, const TestMatrix& mB, SizeType bCol, TestMatrix& result)
		: mA(mA), mB(mB), result(result), aRow(aRow), bCol(bCol)
	{ }

	virtual void execute() {
		result[aRow][bCol] = multiplyRowColumn(mA, aRow, mB, bCol);
	}

private:
	const TestMatrix& mA;
	const TestMatrix& mB;
	TestMatrix& result;
	
	SizeType aRow, bCol;
};

BOOST_AUTO_TEST_CASE( matrixMultiplicationTest ) {
	// initialize matrices and other data:
	
	TestMatrix m0( MATRIX_ORDER, std::vector<Float>(MATRIX_ORDER, 0.0f) );
	TestMatrix m1( MATRIX_ORDER, std::vector<Float>(MATRIX_ORDER, 0.0f) );
	TestMatrix r( MATRIX_ORDER, std::vector<Float>(MATRIX_ORDER, 0.0f) ); // multiplication result
	TestMatrix mt_r( MATRIX_ORDER, std::vector<Float>(MATRIX_ORDER, 0.0f) ); // multithread multiplication result
	
	for (SizeType i = 0u; i < MATRIX_ORDER; ++i) { // initialize matrices
		for (SizeType j = 0u; j < MATRIX_ORDER; ++j) {
			m0[i][j] = Float(MIN_MATRIX_VALUE + ( rand()%(MAX_MATRIX_VALUE-MIN_MATRIX_VALUE) ));
			m1[i][j] = Float(MIN_MATRIX_VALUE + ( rand()%(MAX_MATRIX_VALUE-MIN_MATRIX_VALUE) ));
		}
	}
		
	// initialize thread pool:
	
	ThreadPool pool(5u);
	BOOST_CHECK_EQUAL(pool.getNumberOfThreads(), 5u);

	ThreadPool::TasksGroup group;
		
	for (SizeType i = 0u; i < MATRIX_ORDER; ++i) { // initialize matrices
		for (SizeType j = 0u; j < MATRIX_ORDER; ++j) {
			group.push_back(std::make_unique<ParallelMatrixMult>(m0, i, m1, j, mt_r));
		}
	}
	
	for (SizeType i = 0u; i < 3u; ++i) { // force same execution some times
		// run serial:
		multiply(m0, m1, r);
		
		// run parallel:
		pool.execute(group);
		
		// compare serial and multithread results:
		for (SizeType i = 0u; i < MATRIX_ORDER; ++i) { // initialize matrices
			for (SizeType j = 0u; j < MATRIX_ORDER; ++j) {
				BOOST_CHECK( mt_r[i][j] == r[i][j] );		
			}
		}
	}
	
	// printM(m0);
	// printM(m1);
	// printM(r);
	// printM(mt_r);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
