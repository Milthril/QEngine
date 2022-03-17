#ifndef STDOUTCAPTURE_H
#define STDOUTCAPTURE_H

#include <io.h>
#include <process.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <QString>

class StdCapture {
public:
	StdCapture();
	~StdCapture();
	void BeginCapture();
	bool EndCapture();
	std::string GetCapture() const;
	static StdCapture& getInstance();
	static void start();
	static void stop();
	static QString get();
private:
	enum PIPES { READ, WRITE };
	int m_pipe[2];
	int m_oldStdOut;
	int m_oldStdErr;
	bool m_capturing;
	bool m_init;
	std::string m_captured;
};

#endif // STDOUTCAPTURE_H
