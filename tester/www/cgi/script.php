<?php

echo $_SERVER['REQUEST_METHOD'];
switch($_SERVER['REQUEST_METHOD'])
{
    case 'GET': $the_request = $_GET;
        break;
    case 'POST': $the_request = $_POST;
        break;
    default:
}
print_r($the_request);
?>