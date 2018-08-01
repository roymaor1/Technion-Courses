#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <fstream>
#include <iostream>
#include <sstream>

extern std::string location; // Used for finding the origin of an exception

#define FAIL(message) do { \
	std::ostringstream os; \
	os << __LINE__; \
	location = std::string(__FILE__) + ":" + os.str() + " " + message; \
	return false; \
} while (0)


#define ASSERT_TRUE(statement) do { \
if (!(statement)) { \
	FAIL("statement is false"); \
} \
} while (0)

#define ASSERT_FALSE(statement) do { \
if (statement) { \
	FAIL("statement is true"); \
} \
} while (0)

#define ASSERT_EQUAL(expected,actual) do { \
if (!((expected) == (actual))) { \
	FAIL("values not equal"); \
} \
} while (0)

#define ASSERT_NOT_EQUAL(expected,actual) do { \
if (!((expected) != (actual))) { \
	FAIL("values are equal"); \
} \
} while (0)

#define ASSERT_NO_THROW(command) do {\
	try { \
	(command); \
} \
catch (std::exception& e) { \
	FAIL("caught " + e.what()); \
} \
catch (...) { \
	FAIL("caught unknown object"); \
} \
} while (0)

#define ASSERT_THROW(exception_type,command) do {\
	try { \
	(command); \
	FAIL("missing exception"); \
	} \
	catch (exception_type&) { \
	} \
	catch (std::exception& e) { \
		FAIL("caught " + e.what()); \
	} \
	catch (...) { \
		FAIL("caught unknown object"); \
	} \
} while (0)

#define RUN_TEST(b) do {\
	try { \
	std::cerr << #b << ":"; \
		if (b()) std::cerr << "OK" << std::endl; \
		else std::cerr << "Failed (" << location << ")" << std::endl; \
		} \
		catch (...) { \
			std::cerr << "Failed (" << location << ")" << std::endl; \
		} \
	} while (0)
#define RUN_SETUP(c) do {\
	try { \
		if (!c) return false; \
	}\
	catch (...) { \
		return false; \
	} \
} while (0)

#endif  //TEST_UTILS_H_
