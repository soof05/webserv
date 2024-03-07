#include "request.hpp"

void	request::cgiHandler()
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		std::cout << "fork() has failed" << std::endl;
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/502.html", "502");
	}
	if (pid == 0)
	{
		prepareEnv();
		SetUpInputOutputFiles();
		execve(this->av[0], this->av, this->envp);
		exit(1);
	}
	else if (pid > 0)
	{
		waitForChildProcess(pid);
	}
}

void	request::waitForChildProcess(int pid)
{
	sleep(1);
	int	status = 0;
	int result;
	result  = waitpid(pid, &status, WNOHANG);
	if (result == 0)
	{
		kill(pid, SIGKILL);
		this->endPost = 1;
		this->goToClient("DefaultErrorPages/504.html", "504");
	}
	else
	{
		this->CGI = true;
		this->endPost = 1;
		std::string status = checkIfRedirect();
		this->goToClient(this->outputFile, status);
	}
}

std::string request::checkIfRedirect()
{
	std::string line;
	std::string stats = "200";
	std::ifstream file;

	file.open(this->outputFile, std::ios::in);
	while (getline(file, line))
	{
		if (line.find("Status: ") != std::string::npos)
		{
			stats = "302";
			return stats;
		}
	}
	return stats;
}

void	request::setInterpreterPath()
{
	if (this->scriptExtension == "php")
		this->interpreterPath = "./cgi-bin/php-cgi";
	else if (this->scriptExtension == "pl")
		this->interpreterPath = "/usr/bin/perl";
}

void	request::SetUpInputOutputFiles()
{
	int	output;
	int	input;

	if (this->method == "POST")
	{
		input = open(this->postFileName.c_str(), O_RDONLY, 0777);
		if (input == -1)
			exit(1);
		dup2(input, 0);
		close(input);
	}
	output = open(this->outputFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (output == -1)
		exit(1);
	dup2(output, 1);
	close (output);
}

void	request::prepareEnv()
{
	std::string tmpp;
	std::string			contLength;
	std::stringstream	convert;
	convert << this->contentLenght;
	convert >> contLength;

	setInterpreterPath();

	this->av = new char*[3];
	av[0] = new char[this->interpreterPath.size() + 1];
	strcpy(av[0], this->interpreterPath.c_str());
	av[1] = new char[this->fullPath.size() + 1];
	strcpy(av[1], this->fullPath.c_str());
	av[2] = NULL;

	if (this->method == "POST")
		this->envp = new char*[16];
	else
		this->envp = new char*[14];
	tmpp = "SERVER_PORT=2000";
	this->envp[0] = new char[tmpp.size() + 1];
	strcpy(this->envp[0], (tmpp.c_str()));
	tmpp = "REQUEST_METHOD=" + this->method;
	this->envp[1] = new char[tmpp.size() + 1];
	strcpy(this->envp[1], (tmpp.c_str()));
	tmpp = "REMOTE_ADDR=10.11.11.6";
	this->envp[2] = new char[tmpp.size() + 1];
	strcpy(this->envp[2], (tmpp.c_str()));
	tmpp = "REDIRECT_STATUS=200";
	this->envp[3] = new char[tmpp.size() + 1];
	strcpy(this->envp[3], (tmpp.c_str()));
	tmpp = "QUERY_STRING=" + this->queryString;
	this->envp[4] = new char[tmpp.size() + 1];
	strcpy(this->envp[4], (tmpp.c_str()));
	tmpp = "PATH_INFO=" + this->fullPath;
	this->envp[5] = new char[tmpp.size() + 1];
	strcpy(this->envp[5], (tmpp.c_str()));
	tmpp = "SCRIPT_NAME=" + this->fullPath;
	this->envp[6] = new char[tmpp.size() + 1];
	strcpy(this->envp[6], (tmpp.c_str()));
	tmpp = "SCRIPT_FILENAME=" + this->fullPath;
	this->envp[7] = new char[tmpp.size() + 1];
	strcpy(this->envp[7], (tmpp.c_str()));
	tmpp = "SERVER_PROTOCOL=HTTP/1.1"; 
	this->envp[8] = new char[tmpp.size() + 1];
	strcpy(this->envp[8], (tmpp.c_str()));
	tmpp = "REQUEST_URI=" + this->fullPath;
	this->envp[9] = new char[tmpp.size() + 1];
	strcpy(this->envp[9], (tmpp.c_str()));
	tmpp = "GATEWAY_INTERFACE=CGI/1.1";
	this->envp[10] = new char[tmpp.size() + 1];
	strcpy(this->envp[10], (tmpp.c_str()));
	tmpp = "HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	this->envp[11] = new char[tmpp.size() + 1];
	strcpy(this->envp[11], (tmpp.c_str()));
	tmpp = "HTTP_COOKIE=" + this->cookies;
	this->envp[12] = new char[tmpp.size() + 1];
	strcpy(this->envp[12], (tmpp.c_str()));
	if (this->method == "POST")
	{
		tmpp = "CONTENT_TYPE=" + this->contentType;
		this->envp[13] = new char[tmpp.size() + 1];
		strcpy(this->envp[13], (tmpp.c_str()));
		tmpp = "CONTENT_LENGTH=" + contLength;
		this->envp[14] = new char[tmpp.size() + 1];
		strcpy(this->envp[14], (tmpp.c_str()));
		this->envp[15] = NULL;
	}
	else
		this->envp[13] = NULL;
}
