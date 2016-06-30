<?php

class Dao_NssDB{

	const HOST = '192.168.136.130';
	const PORT = 3306;
	const USER = 'root';
	const PASSWD = 'zhuoliang';
	const DB = 'nss';

	public function __construct(){
		$dsn = 'mysql:host='.self::HOST.';port='.self::PORT.';dbname='.self::DB.';';
		$this->conn = new PDO($dsn, self::USER, self::PASSWD);
	}

	public function query($sql){
		$stat = $this->conn->query($sql);
		$stat->setFetchMode(PDO::FETCH_ASSOC);
		return $stat->fetchAll();
	}

}

?>
