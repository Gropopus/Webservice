/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error_page.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsembel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 11:45:53 by thsembel          #+#    #+#             */
/*   Updated: 2021/12/07 11:47:22 by thsembel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"

std::string		GetErrorPages(std::string type)
{
	std::string page;

	if (type == "/204.html")
		page = "<html>\n\t<head>\n\t<title>Error 204</title>\n\t</head>\n\t\
	<h1>204: No content</h1>\n\t\
	<p>Content has been deleted, there os nothing to be shown.</p>\n\
	</html>";
	else if (type == "/400.html")
		page = "<html>\n\t<head>\n\t<title>Error 400</title>\n\t</head>\n\t\
	<h1>400: Bad request</h1>\n\t\
	<p>Sorry, i don't understand you request.<br/>\
	What s the point of sending this anyway ?</p>\n</html>";
	else if (type == "/401.html")
		page = "<html>\n\t<head>\n\t<title>Error 401</title>\n\t</head>\n\t\
	<h1>401 Unauthorized</h1>\n\t<p>Permission Denied.<br/>\
	Your such a thug, Mummy doesn't want you to see what's in there !</p>\n</html>";
	else if (type == "/403.html")
		page = "<html>\n\t<head>\n\t<title>Error 403</title>\n\t</head>\n\t\
	<h1>403 Forbidden</h1>\n\t\
	<p>We understand your request but the response is NO!<br/>\
	What a terrible kid you are...</p>\n</html>";
	else if (type == "/404.html")
		page = "<html>\n\t<head>\n\t<title>Error 404</title>\n\t</head>\n\t\
	<h1>404: File not found</h1>\n\t<p>The infamous, the notorious.\
	The truth is there is no such content is the server.</p>\n</html>";
	else if (type == "/405.html")
		page = "<html>\n\t<head>\n\t<title>Error 405</title>\n\t</head>\n\t\
	<h1>405 Method Not Allowed</h1>\n\t<p>The rules are the rules.<br/>\
	If the ConfigFile.conf says no, don't force it for god's sake !</p>\n</html>";
	else if (type == "/413.html")
		page = "<html>\n\t<head>\n\t<title>Error 413</title>\n\t</head>\n\t\
	<h1>413 Request Entity Too Large</h1>\n\t<p>OMG you're so big...<br/>\
	Seriously, try sending something smaller</p>\n</html>";
	else if (type == "/500.html")
		page = "<html>\n\t<head>\n\t<title>Error 500</title>\n\t</head>\n\t\
	<h1>500 Internal Server Error</h1>\n\t<p>...Oopsie Doopsie....<br/>\
	Not cool, but still, it s still working!</p>\n</html>";
	else if (type == "/501.html")
		page = "<html>\n\t<head>\n\t<title>Error 501</title>\n\t</head>\n\t\
	<h1>501: Not Implemented</h1>\n\t<p>WTF ?!<br/>Like seriously?\
	<br/>What is that fake ass method ?!</p>\n</html>";
	return (page);
}

std::string		getDefaultIndex(void)
{
	std::string page;

	page = "<head>\n<title>Welcome to webserv!</title>\n\
	<style>\n\tbody {\n\twidth: 35em;\n\tmargin: 0 auto;\n\
		font-family: Tahoma, Verdana, Arial, sans-serif;\n\t}\n\t</style>\n\t\
	</head>\n\t<body>\n\t<h1>Welcome to webserv!</h1>\n\
	<p>If you see this page, the webserv is working i guess</p>\n\t</body>\n\t\
	</html>";

	return (page);
}

void	getErrors(Response &response, Request &request, std::string error)
{
	std::string	path;
	std::ifstream		file;
	std::stringstream	buffer;

	path = request.errors + error;
	file.open(path.c_str(), std::ifstream::in);
	if (file.is_open() == false)
		response.body = GetErrorPages(error);
	else
	{
		buffer << file.rdbuf();
		response.body = buffer.str();
		file.close();
	}
	response.body_len = response.body.size();
	response.content_type = "text/html";
}
