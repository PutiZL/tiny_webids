<?php


/*    @Author: zhuoliang3637@gmain.com    */



class Dao_NssDetecFile{

	private static $tbl_name = 'nss-detec-file';

	public static function retriveNssDetecFileByNssId($nss_id){
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE nss_id='".$nss_id."'";
		$db = new Dao_NssDB();
		return $db->query($sql);
	}


	public static function retriveNssInfoByMd5($md5){
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE md5_hash='".$md5."'";
		$db = new Dao_NssDB();
		return $db->query($sql);
	}

	public static function retriveNssInfoBySha1($sha1){
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE sha1_hash='".$sha1."'";
		$db = new Dao_NssDB();
		return $db->query($sql);
	}


	public static function retriveNssInfoBySha256($sha256){
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE sha256_hash='".$sha256."'";
		$db = new Dao_NssDB();
		return $db->query($sql);
	}
}


?>
