<?php


/*    @Author: zhuoliang3637@gmail.com    */


class Dao_User{
	private static $tbl_name = 'user';

	public static function register($user, $pass){
		$sql = "INSERT INTO `".self::$tbl_name."`(account, passwd) VALUES('".$user."','".$pass."')";
		$db = new Dao_LjrDB();
		return $db->execute($sql);
	}
}


?>
