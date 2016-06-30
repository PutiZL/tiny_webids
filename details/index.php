<?php

define('ROOT', dirname(__FILE__));

$include_path = get_include_path();
$logic_path = ROOT.'/Logic';
$dao_path = ROOT.'/Dao';

/*set_include_path($include_path.PATH_SEPARATOR.$logic_path.PATH_SEPARATOR.$dao_path);*/

function __autoload($classname){
	$path = str_replace('_', '/', $classname);
	include($path.'.php');
}
?>
