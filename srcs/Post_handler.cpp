/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post_handler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaris <gmaris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:41:22 by gmaris            #+#    #+#             */
/*   Updated: 2021/12/17 17:09:27 by gmaris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Web_serv.hpp"
# include "Client.hpp"
# include "Server.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

/*
** Construct response if case of error
*/
void	_construct_error(Response &response, Request &request)
{
	std::ifstream	file;
	std::stringstream	buf;

	response.path = request.errors + "/" + response.status_code.substr(0, response.status_code.find(" "));
	response.path += ".html";
	// std::cout << "path response error = " << response.path << std::endl;
	response.content_type = "text/html";
	file.open(response.path.c_str(), std::ifstream::in);
	if (file.fail())
	{
		response.body = response.status_code;
		response.body_len = response.body.size();
	}
	else
	{
		buf << file.rdbuf();
		response.body = buf.str();
		response.body_len = response.body.size();
		file.close();
	}

}

/*
** Tell if path exist
*/
bool	_isExist(string path)
{
	struct stat	sb;

	if (stat(path.c_str(), &sb) == 0) 
		return true;
	else
		return false;
}

/*
** Tell if path is dir
*/
bool	_isDir(string path)
{
	struct stat	sb;

	stat(path.c_str(), &sb);
	if (S_ISDIR(sb.st_mode))
		return true;
	else
		return false;
}

/*
** Tell if path is exec
*/
bool	_isExec(string path)
{
	struct stat	sb;

	if (stat(path.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR) 
		return true;
	else
		return false;
}

/*
** Prep env for cgi
*/
void	_prep_env(Client &client, std::string path, string name, string env[14])
{
	//server information
	env[0] = "SERVER_SOFTWARE=webserv/1.0";// nom/version
	env[1] = "SERVER_NAME=";
	if (client.request.server_name != "")
		env[1] += client.request.server_name;
	else
		env[1] += "webserv";
	env[2] = "GATEWAY_INTERFACE=CGI/1.1";

	//request information
	env[3] = "SERVER_PROTOCOL=HTTP/1.1";
	env[4] = "SERVER_PORT=";
		env[4] += std::to_string(client.request.config.port);
//NEED FIX HERE
	env[5] = "REQUEST_METHOD=";
		env[5] += client.request.method;
	env[6] = "PATH_INFO="; //optional
	env[7] = "PATH_TRANSLATED=";
		env[7] += path;
	env[8] = "SCRIPT_NAME=";
		env[8] += name;
	env[9] = "REMOTE_HOST=";
		env[9] += client.ip;
	env[10] = "REMOTE_ADDR=";
		env[10] += client.ip;
	if (client.request.method == "POST")
	{
		env[11] = "CONTENT_TYPE=";
		env[11] += client.request.headers["Content-Type"];
	}
	if (client.request.method == "GET")
	{
		env[11] = "QUERY_STRING=";
		env[11] += client.request.query_string; //placeholder need to do
	}

	//client information
	env[12] = "HTTP_ACCEPT=";
		env[12] += client.request.headers["Accept"];
	env[13] = "HTTP_USER_AGENT=";
		env[13] += client.request.headers["User-Agent"];
}

/*
** Free env after use
*/
void	_ft_free_env(char **env)
{
	int i;
	i = 0;
	while (env && env[i])
	{
		free(env[i]);
		env[i] = NULL;
		++i;
	}
	free(env);
}

/*
** Get the output of cmd
*/
std::string		_exec(const char* cmd, Client &client)
{
	pid_t	pid;
	int		old_stdin;
	int		old_stdout;
	string	output;
	string	env[15];
	char **env_c;

	//env prep
	_prep_env(client, cmd, client.request.uri, env);
	int len = 0;
	while (g_env[len])
		++len;
	if ((env_c = (char **)malloc(sizeof(char *) * (len + 15))) == NULL)
		throw std::bad_alloc();
	len = 0;
	while (g_env[len])
	{
		if ((env_c[len] = strdup(g_env[len])) == NULL)
		{
			_ft_free_env(env_c);
			throw std::bad_alloc();
		}
		++len;
	}
	int i = 0;
	while (i < 14)
	{
		if ((env_c[len] = strdup(env[i].c_str())) == NULL)
		{
			_ft_free_env(env_c);
			throw std::bad_alloc();
		}
		++len;
		++i;
	}
	env_c[len] = NULL;

	old_stdin = dup(STDIN_FILENO);
	old_stdout = dup(STDOUT_FILENO);

	FILE	*fin = tmpfile();
	FILE	*fout = tmpfile();
	int		fdin = fileno(fin);
	int		fdout = fileno(fout);

	//put body in tmp file
	write(fdin, client.request.body.c_str(), client.request.body.size());
	lseek(fdin, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
	{
		fclose(fin);
		fclose(fout);
		close(fdin);
		close(fdout);
		close(old_stdin);
		close(old_stdout);
		return (INTERNALERROR);
	}
	if (pid == 0)
	{
		dup2(fdin, STDIN_FILENO);
		dup2(fdout, STDOUT_FILENO);
		execve(cmd, NULL, env_c);
		std::cout << INTERNALERROR;
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
		lseek(fdout, 0, SEEK_SET);
		char buffer[1000];

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1000);
			ret = read(fdout, buffer, 999);
			output += buffer;
		}
	}
	dup2(old_stdin, STDIN_FILENO);
	dup2(old_stdout, STDOUT_FILENO);
	fclose(fin);
	fclose(fout);
	close(fdin);
	close(fdout);
	close(old_stdin);
	close(old_stdout);

	_ft_free_env(env_c);
	return output;
}

/*
** Get path of requested uri
*/
std::string get_path(Request &request)
{
	std::string path;

	if (request.uri.find(request.config.location) != std::string::npos && request.config.location != "/")
		path = request.config.root + request.uri.substr(request.uri.find_last_of('/'));
	else
		path = request.config.root + request.uri;
	//std::cout << "path ask is [" << path << "]\n";
	return (path);
}

/*
** Extract content-type from output
*/

std::string _newOutput(Client &client, std::string output)
{
	int		body_start = output.find("\n\n");
	string	tmp = output.substr(body_start + 2);

	client.response.content_type = output.substr(14, body_start - 14);
	return tmp;
}

/*
** Call the cgi executable if exist
*/
bool	_cgi(Client &client)
{
	string path_exec;
	string output;

	path_exec = get_path(client.request);
	if (_isExist(path_exec) == false)
	{
		client.response.status_code = NOTFOUND;
		_construct_error(client.response, client.request);
		return false;
	}
	if ( _isDir(path_exec) == true)
	{
		client.response.status_code = FORBIDDEN;
		_construct_error(client.response, client.request);
		return false;
	}
	if (_isExec(path_exec) == false)
	{
		client.response.status_code = UNAUTHORIZED;
		_construct_error(client.response, client.request);
		return false;
	}
	//From here the executable exist and can be use
	try
	{
		output = _exec(path_exec.c_str(), client);
		if (output == INTERNALERROR)
		{
			client.response.status_code = INTERNALERROR;
			_construct_error(client.response, client.request);
			return false;
		}
		client.response.body = _newOutput(client, output);
		client.response.body_len = client.response.body.length();
	}
	catch (const std::exception &e)
	{
		client.response.status_code = INTERNALERROR;
		_construct_error(client.response, client.request);
		return false;
	}
	client.response.status_code = OK;
	return true;
}

void	post_handler(Client &client)
{
	if (client.response.status_code != OK)
	{
		_construct_error(client.response, client.request);
		return ;
	}
	if (client.request.config.max_body >= 0 
		&& client.request.headers.find("Content-Length") != client.request.headers.end())
	{
		if (ft_stoi(client.request.headers["Content-Length"]) > client.request.config.max_body)
		{
			client.response.status_code = REQTOOLARGE;
			_construct_error(client.response, client.request);
			return ;
		}
	}
	_cgi(client);

}
