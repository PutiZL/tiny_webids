<?php


/*    @Author: zhuoliang3637@gmail.com    */

include('index.php');

$user = $_GET['user'];
$passwd = $_GET['password'];
Logic_User::register($user, $passwd);




?>
