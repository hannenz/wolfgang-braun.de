<?php
class Model {
	
	protected $tableName;

	protected $primaryKey = 'id';

	protected $db;

	protected $fields;

	protected function __construct() {

		$this->db = new DBCex();

	}


	protected $conditions = array(

		"OR" => array(
			array(
				'field1 =' => 'foo',
				'field2 =' => 'bar'
			),
			'field3 =' => 'baz'
		)
	);



	public function find($type, $conditions = array(), $order = array(), $limit = -1) {

		$query = sprintf('SELECT * FROM %s', $this->tableName);

		if (!empty($conditions)) {
			foreach ($conditions as $condition) {

			}

		};

	}

	protected function buildConditionPart($conditions) {

		$key = key($conditions);
		switch (strtoupper($key)) {
			case 'OR':
				break;
			case 'AND':
				break;
			case 'NOT':
				break;
			default:

				$statement .= $this->buildCondition($key, $conditions);
				break;
		}
	}

	protected function buildCondition($left, $right) {

		$str = '';

		preg_match('/^(\w+)\s+?(.*)?$/', $left, $matches);
		$operand1 = trim($matches[1]);
		$operator = !empty($matches[2]) ? trim($matches[2]) : '=';
		$operand2 = trim($right);


		switch (strtoupper($operator)) {
			case '=':
			case '!=':
			case '<':
			case '>':
			case '<=':
			case '>=':
			case '<=>':
			case 'LIKE':
			case 'REGEXP':
			case 'IS':
			case 'IS NOT':
				$str = sprintf("%s %s", $operand1, $operator, $operand2);
				break;

			case 'BETWEEN':
			case 'NOT BETWEEN':
				if (!is_array($right) || count($right) < 2) {
					// error
				}
				$str = sprintf("%s %s %s and %s", $operand1, $operator, $right[0], $right[1]);
				break;

			case 'IN':
			case 'NOT IN':
				if (!is_array($right)) {
					// error
				}
				$str = sprintf("%s %s (%s)", $operand1, $operator, join(',', $right));
				break;

		}

		return $str;
	}
}
