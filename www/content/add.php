<?php

echo "Content-Type: text/html\r\n\r\n";
echo $_REQUEST['num1'], ' + ', $_REQUEST['num2'], ' = ';
echo $_REQUEST['num1'] + $_REQUEST['num2'];

?>