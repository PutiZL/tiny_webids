<?php


/*    @Author: zhuoliang3637@gmail.com    */


class Logic_User{
	public static function register($user, $pass){
		return Dao_User::register($user, $pass);
	}
}




?>
