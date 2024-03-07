#include "respond.hpp"

respond::respond()
{
}

respond::respond(const respond &obj)
{
    *this = obj;
}

respond& respond::operator=(const respond &obj)
{
    this->fileToGet = obj.fileToGet;
    this->statusRespond = obj.statusRespond;
	this->clientFd = obj.clientFd;
	this->fileFd = obj.fileFd;
	this->lenToSend = obj.lenToSend;
	this->bufferSend =  obj.bufferSend;
	// this->numberRedirectionSend = obj.numberRedirectionSend;
    return *this;
}

//set Headers to send them to client
void	respond::sendHeadersToClient()
{
	std::string returnStatOfRequest;
	std::string contentType = "Content-Type: " + findContentType();
    std::string contentLength = findLengthOfFile();

	if (this->statusRespond == 301)
	{
		this->initResponseType();
		returnStatOfRequest = "HTTP/1.1 " + std::to_string(this->statusRespond) + " Moved Permanently\r\n" + "Location: " + this->redirection + "\r\n";
	}
	else{
		this->initResponseType();
		returnStatOfRequest = "HTTP/1.1 " + std::to_string(this->statusRespond) + this->responseType +"\r\n";
	}
	// }
	// if (contentType != "Content-Type: NO_Extention")
	// {
		this->bufferSend = returnStatOfRequest + contentType + contentLength;
		// std::cout << "Respond Header : " << this->bufferSend << std::endl;
		this->lenToSend = returnStatOfRequest.size() + contentType.size() + contentLength.size();
	// }
	// else
	// {
	// 	this->bufferSend = returnStatOfRequest + contentLength;
	// 	std::cout << "Respond Header : " << this->bufferSend << std::endl;
	// 	this->lenToSend = returnStatOfRequest.size() + contentLength.size();
	// }
}

void	respond::initResponseType()
{
	switch (this->statusRespond)
	{
		case 404:
			this->responseType = " Not Found";
			break;
		case 403:
			this->responseType = " Forbidden";
			break;
		case 400:
			this->responseType = " Bad Request";
			break;
		case 405:
			this->responseType = " Method Not Allowed";
			break;
		case 409:
			this->responseType = " Conflict";
			break;
		case 413:
			this->responseType = " Request Entity Too Large";
			break;
		case 200:
			this->responseType = " OK";
			break;
		default:
			this->responseType = "";
	}
	return;
}

int isExtension(std::string& filename, std::string& fileExtension)
{
    // List of common image extensions
    const std::string imageExtensions[] = {".jpg", ".jpeg", ".png", ".gif", ".bmp"};
	//List of common image extensions
	const std::string videoExtensions[] = {".mp4", ".avi", ".mov", ".mkv", ".wmv", ".flv", ".m4v", ".mpg", ".webm"};

    // Get the last period in the filename
    size_t lastPeriodPos = filename.find_last_of('.');

    if (lastPeriodPos != std::string::npos)
    {
        // Extract the extension from the filename
        fileExtension = filename.substr(lastPeriodPos);

        // Check if the extension matches any image extension
        for (int i = 0; i < 5; i++)
            if (fileExtension == imageExtensions[i])
			{
				fileExtension = filename.substr(lastPeriodPos+1);
                return 1; // Extension found
			}
	    
		// Check if the extension matches any video extension
		for (int i = 0; i < 9; i++)
            if (fileExtension == videoExtensions[i])
			{
				fileExtension = filename.substr(lastPeriodPos+1);
                return 2; // Extension found
			}
		
		// Check if the extension matches html extension
		if (fileExtension == ".html")
		{
			fileExtension = filename.substr(lastPeriodPos+1);
			return 3;
		}
	}
	fileExtension = "text/html\r\n";
    return 4; // No image extension found
}

std::string	respond::findContentType()
{
	std::string	fileExtension;
	int flagToExtension = isExtension(this->fileToGet, fileExtension);
	// std::cout << "FIle extension : " << fileExtension << "\n";
	std::string	contentType;
	//if no extention u don't have to return content type in the headers;
	if (flagToExtension == 4){
		return fileExtension;
	}
	if (flagToExtension == 1){
		contentType = "image/" + fileExtension;
	}
	if (flagToExtension == 2){
		contentType = "video/" + fileExtension;
	}
	if (flagToExtension == 3){
		contentType = "text/" + fileExtension;
	}
	return contentType + "\r\n";
}

std::string respond::findLengthOfFile()
{
	std::string length;
    struct stat fileInfo;
    const char* filename = this->fileToGet.c_str();
	// this->openFileToserve();
	this->fileFd = open(this->fileToGet.c_str(), O_RDONLY);
	if (this->fileFd < 0)
	{
		// std::cout << "ALFILE ASHBI\n";
		this->statusRespond = 404;
		return	"Content-Length: 500\r\n\r\n";
	}
	if (stat(filename, &fileInfo) != 0)
	{
		// std::cout << "FILE NAME IS : " << filename << std::endl;
		// std::cout << "Error retrieving file size.\n";
        throw Exception ("Error retrieving file size.");
	}
	if (!this->CGI)
    	length = "Content-Length: " + std::to_string(fileInfo.st_size) + "\r\n\r\n";
	else
		length = "Content-Length: " + std::to_string(fileInfo.st_size);
    return length;
}

void	respond::openFileToServe()
{
	//get fd for the file client request (get Method);
	//  this->fileFd = open("file.html", O_CREAT | O_WRONLY);
	this->fileFd = open(this->fileToGet.c_str(), O_RDONLY);
	if (this->fileFd < 0)
	{
		// std::cout << "Error In Open File\n";
        exit(1);
	}
}
