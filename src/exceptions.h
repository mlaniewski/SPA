#ifndef SRC_EXCEPTIONS_H_
#define SRC_EXCEPTIONS_H_

class Exception {
public:
	int line;
	string parser;
	string msg;
	bool runtime = false;

	void print() {
		stringstream ss;
		ss << "B³¹d zwi¹zany jest z lini¹ " << line << ", " << parser << ", " << msg << endl;
		cout << ss.str();
	}

};

class RuntimeException: public Exception {
public:
	RuntimeException() {
		runtime = true;
	}

	RuntimeException(int line, string parser, string msg) :
			RuntimeException() {
		this->line = line;
		this->parser = parser;
		this->msg = msg;
	}
};

#endif /* SRC_EXCEPTIONS_H_ */
