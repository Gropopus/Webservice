#!/usr/bin/python
import cgi, os
import cgitb; cgitb.enable()
form = cgi.FieldStorage()
# Get filename here.
fileitem = form['filename']

path_trans =  os.getenv('PATH_TRANSLATED')
len_path = len(path_trans)
script_name = os.getenv('SCRIPT_NAME')
pos = script_name.rfind('/')
script_name = script_name[pos:]
len_name = len(script_name)
len_print = len_path - len_name
path_trans = path_trans[0:len_print]
path_trans += '/fileupload/'

# Test if the file was uploaded
if fileitem.filename:
   # strip leading path from file name to avoid
   # directory traversal attacks
   fn = os.path.basename(fileitem.filename)
   open(path_trans + fn, 'wb').write(fileitem.file.read())
   message = 'The file "' + fn + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'
 
tmp = os.getenv('SCRIPT_NAME')
pos = script_name.rfind('/')
new_name = script_name[0:pos]

url = '<p><a href="http://localhost:'
url += os.getenv('SERVER_PORT')
url += '/'
url += new_name
url += '">link</a></p>'

print """\
Content-Type: text/html\n
<html>
<body>
	%s
   <p>%s</p>
</body>
</html>
""" % (url, message,)