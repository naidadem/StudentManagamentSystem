#include <iostream>
using namespace std;

#pragma warning(disable:4996)

char * crt = "\n----------------------------------------------------\n";
class DateD {
	int * _day, *_month, *_year;
public:
	DateD() { _day = new int(0); _month = new int(0); _year = new int(0); }
	DateD(int d, int m, int y) {
		_day = new int(d);
		_month = new int(m);
		_year = new int(y);
	}
	DateD(const DateD & d) {
		_day = new int(*d._day);
		_month = new int(*d._month);
		_year = new int(*d._year);
	}
	DateD & operator=(const DateD & d) {
		if (this != &d) {
			delete _day;
			delete _month;
			delete _year;
			_day = new int(*d._day);
			_month = new int(*d._month);
			_year = new int(*d._year);
		}
		return *this;
	}
	bool operator==(DateD & d) {
		return *_day == *d._day && *_month == *d._month && *_year == *d._year;
	}
	~DateD() {
		delete _day; _day = nullptr;
		delete _month; _month = nullptr;
		delete _year; _year = nullptr;
	}
	friend ostream & operator<<(ostream & COUT, DateD obj) {
		COUT << *obj._day << "/" << *obj._month << "/" << *obj._year;
		return COUT;
	}
};

template<class T, int size>
class FITArray {
	T _array[size];
	int _currently;
	bool _allowDuplicating;
public:
	//DEFINE OTHER FUNCTIONS OUTSIDE TEMPLATE CLASS
	bool operator+=(T);
	void operator-=(T);
	FITArray(const FITArray &);
	FITArray & operator()(int, int);
	FITArray(bool allowDuplicating = true) { _allowDuplicating = allowDuplicating; _currently = 0; }
	int GetCurrently() { return _currently; }
	T * GetArray() { return _array; }
};
template<typename T, int max>
bool FITArray<T, max>::operator+=(T a) {
	if (_currently > max)
		return false;
	for (int i = 0; i < _currently; i++) {
		if (_array[i] == a)
			return false;
	}
	_array[_currently] = a;
	_currently++;
	return true;
}
template<typename T, int max>
ostream & operator<<(ostream & cout, FITArray<T, max> f) {
	for (int i = 0; i < f.GetCurrently(); i++)
		cout << f.GetArray()[i] << endl;
	return cout;
}
template<typename T, int max>
void FITArray<T, max>::operator-=(T a) {
	int location = -1;
	for (int i = 0; i < _currently; i++) {
		if (_array[i] == a)
			location = i;
	}
	if (location == -1) {
		cout << "There is no element in the arry with that value" << endl;
		return;
	}
	for (int i = location; i < _currently - 1; i++)
		_array[i] = _array[i + 1];
	_currently--;
}
template<typename T, int max>
FITArray<T, max>::FITArray(const FITArray<T, max> & f) {
	_currently = f._currently;
	for (int i = 0; i < _currently; i++)
		_array[i] = f._array[i];
}
template<typename T, int max>
FITArray<T, max> & FITArray<T, max>::operator()(int FROM, int TO) {
	FITArray * new = new FITArray<T, max>;
	if (TO>_currently) {
		for (int i = FROM; i < _currently; i++)
			*new += _array[i];
	}
	else {
		for (int i = FROM; i < TO; i++)
			*new += _array[i];
	}
	return *new;
}

/* parameters determine the locations (indexes in a row) of the elements FROM - TO that need to be returned.
in the case that there is no wanted element number function  should return all elements from the location to the last available element */

class PassedCourse {
	char * _title;
	int _grade;
	DateD _DateOfTakingExam;
public:
	PassedCourse() {
		_title = nullptr;
	}
	PassedCourse(DateD d, char * title, int grade) {
		_title = new char[strlen(title) + 1];
		strcpy_s(_title, strlen(title) + 1, title);
		_DateOfTakingExam = d;
		_grade = grade;
	}
	PassedCourse(const PassedCourse & p) {
		_title = new char[strlen(p._title) + 1];
		strcpy_s(_title, strlen(p._title) + 1, p._title);
		_DateOfTakingExam = p._DateOfTakingExam;
		_grade = p._grade;
	}
	PassedCourse & operator=(const PassedCourse & p) {
		if (this != &p) {
			delete[] _title;
			_title = new char[strlen(p._title) + 1];
			strcpy_s(_title, strlen(p._title) + 1, p._title);
			_DateOfTakingExam = p._DateOfTakingExam;
			_grade = p._grade;
		}
		return *this;
	}
	bool operator==(PassedCourse & p) {
		return strcmp(_title, p._title) == 0 && _grade == p._grade;
	}
	int getGrade() { return _grade; }
	~PassedCourse() { delete[] _title; _title = nullptr; }
	friend ostream & operator<<(ostream & COUT, PassedCourse & obj) {
		COUT << obj._title << "(" << obj._grade << ")" << obj._DateOfTakingExam << endl;
		return COUT;
	}
};

class Student {
	static int _indexCounter; //determining the next index number that will be awarded to the student, starting with 160000
	const int _index;
	char * _name;
	FITArray<PassedCourse, 40> _PassedCourses;
public:
	Student(char * ip) :_index(_indexCounter++) {
		_name = new char[strlen(ip) + 1];
		strcpy_s(_name, strlen(ip) + 1, ip);
	}
	Student(Student & s) :_index(_indexCounter++) {
		_name = new char[strlen(s._name) + 1];
		strcpy_s(_name, strlen(s._name) + 1, s._name);
		for (int i = 0; i < s._PassedCourses.GetCurrently(); i++)
			_PassedCourses += s._PassedCourses.GetArray()[i];
	}
	Student & operator=(Student & s) {
		if (this != &s) {
			delete[] _name;
			_name = new char[strlen(s._name) + 1];
			strcpy_s(_name, strlen(s._name) + 1, s._name);
			for (int i = 0; i < s._PassedCourses.GetCurrently(); i++)
				_PassedCourses += s._PassedCourses.GetArray()[i];
		}return *this;
	}
	~Student() {
		delete[] _name; _name = nullptr;
	}
	void AddCourse(PassedCourse & p) {
		_PassedCourses += p;
	}
	float GetGPA() {
		float GPA = 0;
		for (int i = 0; i < _PassedCourses.GetCurrently(); i++)
			GPA += _PassedCourses.GetArray()[i].getGrade();
		return GPA / _PassedCourses.GetCurrently();
	}
	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._index << " " << obj._name << endl << obj._PassedCourses;
		return COUT;
	}
};
int Student::_indexCounter = 160000;

void main() {
	const int max = 20;

	DateD yesterday(20, 11, 2017), today(21, 11, 2017), tomorrow(22, 11, 2017);
	cout << yesterday << today << tomorrow << endl;
	yesterday = today;
	cout << yesterday << today << tomorrow << endl;

	//bool type parameter that determines if duplicating elements is allowed in the array
	FITArray<int, max> arrayOfIntegers(false);

	for (size_t i = 0; i < max - 1; i++)
		arrayOfIntegers += i;//adding new member in the array

	if (!(arrayOfIntegers += 6))//trying to add duplicate element in the array
		cout << "Element hasn't been added into array" << endl;

	cout << crt << arrayOfIntegers << crt; //printing all elements of the array

	arrayOfIntegers -= 9; //removing element from the array with value 9
	arrayOfIntegers -= 17;

	FITArray<int, max> newarrayOfIntegers(arrayOfIntegers);

	cout << crt << newarrayOfIntegers << crt;

	/*parameters determine the locations(indexes in the array) of the elements FROM - TO that need to be reset. In case there is no wanted 
	number of elements function  should return all elements from the location to the last available element */
	cout << "Elements of the array from location 2 to location 5 are -> " << newarrayOfIntegers(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PassedCourse prII(yesterday, "PRII", 8), prIII(tomorrow, "PRIII", 7);

	adel.AddCourse(prII);
	adel.AddCourse(prIII);
	cout << adel << endl;
	//returns GPA of student
	cout << "GPA -> " << adel.GetGPA() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "GPA -> " << jasmin.GetGPA() << crt;
	system("pause");
}