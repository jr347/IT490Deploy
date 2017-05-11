<?php

require_once '/usr/share/php/PhpAmqpLib/autoload.php';
use PhpAmqpLib\Connection\AMQPStreamConnection;
use PhpAmqpLib\Message\AMQPMessage;

class DeployClient {
	private $connection;
	private $channel_send;
	private $channel_receive;
	private $callback_queue;
	private $response;
	private $corr_id;
	private $timeout;

	public function __construct() {
		$this->connection = new AMQPStreamConnection('192.168.43.125', 5672, 'IT490', '12345', 'Login');
		$this->channel_send = $this->connection->channel();
		$this->channel_receive = $this->connection->channel();
		$this->channel_send->queue_declare('beprod_send', false, true, false, false);
		$this->channel_receive->queue_declare('beprod_receive', false, true, false, false);
		$this->channel_receive->basic_consume($this->callback_queue, 'beprod_receive', false, true, false, false, array($this, 'on_response'));
		echo $this->channel_send->getChannelId();
	
	}
	
	public function on_response($rep) {
		if($rep->get('correlation_id') == $this->corr_id){
			$this->response = $rep->body;
		}
	}
	
	public function call($n) {
		$this->response = null;
		$this->corr_id = uniqid();
		$this->timeout = 1;
		$msg = new AMQPMessage(
			(string) $n,
			array('correlation_id' => $this->corr_id,
				'reply_to' => $this->callback_queue)
		);
		$this->channel_send->basic_publish($msg, '', 'beprod_send');
		while(!$this->response){
			try{
			$this->channel_receive->wait(null, false, $this->timeout);
		}catch(\PhpAmqpLib\Exception\AMQPTimeoutException $e){
			$this->channel_receive->close();
			exit;
		}
		}
		$this->channel_send->basic_publish($msg, '', 'beprod_send');
		$resp = $this->response;
		return $resp;
	}
};

?>
