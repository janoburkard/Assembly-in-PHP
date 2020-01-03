<?php

class assembly {
	
     
     public $registers = array( "EAX" => 0 , "ECX" => 0, "EDX" => 0, "EBX" => 0, "ESP" => 0, "EBP" => 0, "ESI" => 0, "EDI" => 0, "EIP" => 0); //, "CS" => 0 , "SS" =>0, "DS" => 0, "ES" => 0 );
     
     public $stack = array();
     
     public $memory = array();
         
     public $debug = 0;
     
     
     /*
     Registradores:
     -EAX: é o chamado ACUMULADOR
     -EBX: É o chamado registrador de BASE, pois pode ser usado como base de endereçamento para manipulação de tabelas.
     -ECX: É chamado registrador de CONTAGEM, pois além de trabalhar com dados também é usado para contagens em instruções com “LOOPS” na manipulação de strings.
     -EDX: É chamado de registrador de DADOS, pois ele pode ser usado como uma extensão do EAX em operações de multiplicação e divisão.
     -ESP: é o STACK POINTER ou ponteiro de pilhas, que aponta para o topo da memória do tipo pilha usada pelo 8088.
     -EBP: este registrador, que é chamado de BASE POINTER, é usado para endereçamento de memória (geralmente relacionado a pilha, ou seja, é uma registrador de base para a pilha). Pode conter o endereço inicial (base) de uma tabela que será manipulada pela CPU.
     -ESI: SOURCE INDEX (registrador de índice que aponta para a origem dos dados que serão movimentados) é usado para indexação de tabelas no endereçamento indireto.
     -EDI: DESTINATION INDEX (registrador de índice que aponta para o destino dos dados que serão movimentados). Assim como o SI, o registrador DI é utilizado para a indexação no endereçamento indireto.
     
     Registradores de Flags:
     -PF (Parity flag): indica se o número de bits definidos é ímpar ou par na representação binária do resultado da última operação. Bit (1 = Par, 0 = impar).
     -ZF (Zero Flag): indica se o resultado de uma operação aritmética é igual a zero (1) ou diferente de zero (0). Se numa operação os valores são iguais o ZF = 1.
     -SF (Sign Flag): indica se o resultado de uma operação com sinal é positivo (0), em caso contrário (1).
     -TF (Trap Flag): permite a operação “passo a passo” para debug.
     -IF (Interrupt Flag): indica se as interrupções estão habilitadas (1) ou não (0).
     -DF (Direction Flag): para operações com strings, se D=0 os registradores de índice serão incrementados, caso contrário (D=1) serão decrementados.
     -OF (Overflow Flag): indica um estouro da capacidade de armazenamento de um registrador.
     
     Registradores de Segmento:
     -CS (CODE SEGMENT): endereça o segmento onde devem estar as intruções a serem executadas pela CPU
     -SS (STACK SEGMENT): endereça o segmento que está reservado ao uso da PILHA ou STACK.
     -DS (DATA SEGMENT): acesso o segmento resrvado aos dados manipulados pelo 8088.
     -ES (EXTRA SEGMENT): é usado para acessar outro segmento que conterá dados. Normalmente, é usado para manipulação de tabelas e strings.
     */
     
	public $flag_carry = 0;
	public $flag_parity = 0;
	public $flag_zero = 0;
	public $flag_signal = 0;
	public $flag_overflow = 0;

	public $reserva = 0;

	
     function show_stack()
     {

		$esp = $this->get_register("ESP");
				
		echo "\r\n\r\n".count( $this->stack ) . "  -   ". $esp;		

		echo "\r\n\r\n------- STACK ------\r\n\r\n";
		
		for($i=count( $this->stack ); $i > 0; $i-=4)
		{
			echo str_pad( base_convert( $esp, 10, 16) , 8, "0", STR_PAD_LEFT) ." => ";
			echo str_pad( base_convert( $this->stack[$esp], 10, 16) , 2, "0", STR_PAD_LEFT);
			echo str_pad( base_convert( $this->stack[$esp+1], 10, 16) , 2, "0", STR_PAD_LEFT);
			echo str_pad( base_convert( $this->stack[$esp+2], 10, 16) , 2, "0", STR_PAD_LEFT);
			echo str_pad( base_convert( $this->stack[$esp+3], 10, 16) , 2, "0", STR_PAD_LEFT);  
			$esp+=4;
			echo "\r\n";
		}
		echo "\r\n====================\r\n";					

     }	

	function show_memory()
	{
		echo "\r\n ------ MEMORIA ---\r\n\r\n";
		foreach($this->memory as $key => $val)
			echo str_pad( base_convert( $key , 10, 16) , 8 , '0', STR_PAD_LEFT) ." = ". str_pad( base_convert( $val , 10, 16) , 2 , '0', STR_PAD_LEFT)." - ".chr($val)."\r\n";
		echo "\r\n -------------------\r\n\r\n";	
	}
	
     function show_registers()
     {
		
		echo "\r\n\r\n------- REGISTERS ------\r\n\r\n";
		foreach($this->registers as $key => $val)
			echo "{$key} = ". strtoupper( str_pad( base_convert( $val , 10, 16) , 8, "0", STR_PAD_LEFT) ). "   Decimal: {$val}\r\n\r\n";
		echo "\r\n====================\r\n";					

     }
				     
	function get_register($opcode)
	{                                          

		if (strpos("EAX|EBX|ECX|EDX|AH|AL|BH|BL|CH|CL|DH|DL|ESP|EBP|ESI|EDI|EIP", $opcode ) !== False)
		{
						
			$i = strlen($opcode) - 2;
		
			$ro = ($i == 0)?$opcode[1]:$opcode[0];



			if (strpos("|EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", '|'.$opcode.'|' ) !== False)
			{ 
				$data = $this->registers[$opcode];
				$md = "E"; 		
			} 		
			else if(isset($this->registers["E".$opcode[$i]."X"]))
				$data = $this->registers["E".$opcode[$i]."X"];
			else
			{
				// NÃO É OS REGISTRADOS EAX, EBX, ECX OU EDX
				if(!isset($this->registers[$opcode])) die ("\r\nRegister \"{$opcode}\" does not exist.");
			
				$data = $this->registers[$opcode];
				$ro = "E";
			}
			
			
			$abin  =  str_pad( base_convert( $data ,10,2) , 32, '0', STR_PAD_LEFT);	
				
			$binario = "";
		
			if (strpos("LH", $ro) !== False)
			{
				if (strpos("L", $ro) !== False)
					$binario = substr($abin,24,8);
				else
					$binario = substr($abin,16,8);
			}
			if (strpos("X", $ro) !== False)
			{
				$binario  = substr($abin,16,8);
				$binario .= substr($abin,24,8);
			}
			if (strpos("E", $ro) !== False)
			{
				$binario  = substr($abin,0,8);		 
				$binario .= substr($abin,8,8);		 
				$binario .= substr($abin,16,8);		 
				$binario .= substr($abin,24,8);	
			}
		
           
           	if($this->debug) echo "\r\n GET->{$opcode} = ".base_convert($binario, 2, 16);
           
			$ret = bindec($binario);
            	
            	// Fim pegar dados de registradores
			
		}
		else if( preg_match("/(BYTE|WORD|DWORD) PTR (DS|SS):\[(.*)\]/", $opcode, $match1))
		{
			if ($this->debug) print_r($match1);
			
			if( preg_match("/(LOCAL)[.]([0-9]*)/", $match1[3], $match2) )
			{
				if ($this->debug) print_r( $match2 );
				
				if($this->debug) $this->show_stack();
				
				if( $match1[2] == "SS" )
				{   
					// SS - Stack


						reset($this->stack); // Vai para o primeiro elemento ou seja o final da pilha
			     			      
						$address = key($this->stack) - 3 - (4 * base_convert( $match2[2], 10,10)); // 4 é o tamanho maximo armazenado DWORD que é 4 bytes
				
						end($this->stack);
						
						if($this->debug) echo "\r\n##### address: ".base_convert( $address ,10, 16)."\r\n";

						$offset = $this->stack;
					
					if (!isset( $this->stack[ $address ] )) die("\r\n\r\nEnderecamento de pilha invalido. ESP = \"".base_convert( $address ,10, 16)."\" - ESP Decimal = \"{$address}\"");
             }
             else
             {
                // DS - Memory
                      	
                $address = $this->get_register("EDI"); 
                      	
                $offset = $this->memory;                      	
             }
                      
				$byte0 = str_pad( decbin( $offset[$address] ) , 8, "0", STR_PAD_LEFT);
            $byte1 = str_pad( decbin( $offset[$address+1] ) , 8, "0", STR_PAD_LEFT);
            $byte2 = str_pad( decbin( $offset[$address+2] ) , 8, "0", STR_PAD_LEFT);
            $byte3 = str_pad( decbin( $offset[$address+3] ) , 8, "0", STR_PAD_LEFT); 
				
				$ret = bindec( $byte3.$byte2.$byte1.$byte0);
			}
			else
			{


				if (preg_match_all("/([0-9A-X]{1,8})(.)?/", $match1[3], $match2))
				{
					$val = 0;
										
					// $this->show_memory();			
					
					if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][0]) !== False) // Verifica se eh registrador
						$val = $this->get_register( $match2[1][0] );
					else
						$val = bindec( base_convert( $match2[1][0], 16, 2) ); // Valor hexadecimal
						
					 //echo "\r\nval = ".base_convert( $val, 10, 16)." - val = {$val}\r\n";

                           
					$ret = $val;
                           
					if(isset($match2[1][1]))
					{
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][1]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][1] );
						else
							$val = bindec( base_convert( $match2[1][1], 16, 2) ); // Valor hexadecimal
							
						$ret = $this->_inc_2_($ret, $val , "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
					}
					
					//echo "\r\nval = ".base_convert( $ret, 10, 16)." - val = {$val}\r\n";
					//print_r($match2);
					//die("");
										
					if(isset($match2[1][2]))
					{
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][2]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][2] );
						else
							$val = bindec( base_convert( $match2[1][2], 16, 2) ); // Valor hexadecimal
							
						$ret = $this->_inc_2_($ret, $val, "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
					}
					
					// echo "\r\nval = ".base_convert( $ret, 10, 16)." - val = {$ret}\r\n"; 

					if( $match1[2] == "SS" )
					{   
						// SS - Stack
					
						$address = $ret;

						$offset = $this->stack;
					
						if (!isset( $this->stack[ $address ] )) die("\r\n\r\nEnderecamento de pilha invalido. ESP = \"".base_convert( $address ,10, 16)."\" - ESP Decimal = \"{$address}\"");
					}
					else
					{
                   // DS - Memory
                      	
                   $address = $ret ; // 
                      	
                   $offset = $this->memory;
                      	
					}
                      
					$byte0 = str_pad( decbin( $offset[$address] ) , 8, "0", STR_PAD_LEFT);
               $byte1 = str_pad( decbin( $offset[$address+1] ) , 8, "0", STR_PAD_LEFT);
               $byte2 = str_pad( decbin( $offset[$address+2] ) , 8, "0", STR_PAD_LEFT);
               $byte3 = str_pad( decbin( $offset[$address+3] ) , 8, "0", STR_PAD_LEFT);
               			
               //echo "\r\n -------------------------> ". base_convert( $address+3 , 10, 16)." ". base_convert( $address+2 , 10, 16)." ". base_convert( $address+1 , 10, 16)." ". base_convert( $address , 10, 16)." \r\n"; 
               //echo "\r\n -------------------------> ". base_convert( $offset[$address+3] , 10, 16)." ". base_convert( $offset[$address+2] , 10, 16)." ". base_convert( $offset[$address+1] , 10, 16)." ". base_convert( $offset[$address] , 10, 16)." \r\n"; 
               							
					$ret = bindec( $byte3.$byte2.$byte1.$byte0);

					
					if ($this->debug) print_r( $match2 );
				}
				else
					die("\r\nOpcode \"{$opcode}\" invalido\r\n");				

				
			}
		}
		else if( preg_match("/\[(.*)\]/", $opcode, $match1))
		{                                             

               if( preg_match("/(LOCAL)[.]([0-9]*)/", $match1[1], $match2) )
               {
						reset($this->stack); // Vai para o primeiro elemento ou seja o final da pilha
			     			      
						$ret = key($this->stack) - 3 - (4 * base_convert( $match2[2], 10,10)); // 4 é o tamanho maximo armazenado DWORD que é 4 bytes
				
						end($this->stack);
				
               }
					else if (preg_match_all("/([0-9A-X]{1,8})(.)?/", $match1[1], $match2))
					{                                                                             
		            // codigo duplicado fazer ele virar uma function
               
              		// print_r($match2);
                 
						$val = 0;
					
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][0]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][0] );
						else
							$val = bindec(base_convert( $match2[1][0], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
                           
                  	 $ret = $val;
						// echo "\r\n---------------------------> ESP = ".base_convert($ret, 10, 16)."\r\n"; // 
						                           
						if(isset($match2[1][1]))
						{
							if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][1]) !== False) // Verifica se eh registrador
								$val = $this->get_register( $match2[1][1] );
							else
								$val = bindec(base_convert( $match2[1][1], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
							
							$ret = $this->_inc_2_($ret, $val, "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
						}
						//echo "\r\n---------------------------> ESP = ".base_convert($ret, 10, 16)."\r\n"; // 					
						if(isset($match2[1][2]))
						{
							if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][2]) !== False) // Verifica se eh registrador
								$val = $this->get_register( $match2[1][2] );
							else
							{
								$val = bindec(base_convert( $match2[1][2], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
								// echo "\r\n {$match2[1][2]} - {$val} \r\n";
							} 

							$ret = $this->_inc_2_($ret, $val , "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
							
						}
						
						//echo "\r\n---------------------------> ESP = ".$ret."\r\n"; // 
		    	//
		    }

		}		
		else
		{
			die("\r\nOpcode para acesso de memoria invalido. {$opcode}");
		}
		
		return ($ret);

	}

	function set_register($opcode, $value)
	{                               
		
		$md = $this->get_register($opcode); // Obtem o valor antes da troca - DESTINO		
		
		$mo = $value; // Valor de ORIGEM


		if (strpos("EAX|EBX|ECX|EDX|AH|AL|BH|BL|CH|CL|DH|DL|ESP|EBP|ESI|EDI|EIP", $opcode ) !== False)
		{
                
			$mo = $value;
			
			$i = strlen($opcode) - 2;
  	
			$ro = ($i == 0)?$opcode[1]:$opcode[0];
			
			if (strpos("|EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP|", '|'.$opcode.'|') !== False)
			{ 
				$nm_reg = $opcode;
				$md = $this->registers[$nm_reg];		
			} 		
			else if(isset($this->registers["E".$opcode[$i]."X"]))
			{
				$nm_reg = "E".$opcode[$i]."X";
				$md = $this->registers[$nm_reg];
			}
			else
			{
				// NÃO É OS REGISTRADOS EAX, EBX, ECX OU EDX
				if(!isset($this->registers[$opcode])) die ("Nao posso gravar dados no registrador \"{$opcode}\" pois ele não existe");
				$nm_reg = $opcode;
				$md = $this->registers[$opcode];
				$ro = "E";
			}
							
			$byte0 = $md & 0xFF;
			$byte1 = $md >> 8 & 0xFF;
			$byte2 = $md >> 16 & 0xFF;
			$byte3 = $md >> 24 & 0xFF;
			
         
			if (strpos("LH", $ro) !== False)
			{
				if (strpos("L", $ro) !== False)
					$byte0 =  ($mo & 0xFF);
				else
					$byte1 =  ($mo & 0xFF);				
			}
			if (strpos("XE", $ro) !== False)
			{
					$byte0 =  ($mo & 0xFF);
					$byte1 =  ($mo >> 8 & 0xFF);				
			}
			if (strpos("E", $ro) !== False)
			{
					$byte2 =  ($mo >> 16 & 0xFF);
					$byte3 =  ($mo >> 24 & 0xFF);				
			}			

			if( $this->debug) echo "\r\nArmazena em {$register} = ". base_convert( $byte3 , 10, 16)." - ".base_convert( $byte2 , 10, 16)." - ".base_convert( $byte1 , 10, 16)." - ".base_convert( $byte0 , 10, 16);

			$ret = bindec( str_pad( decbin($byte3) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte2) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte1) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte0) ,8, '0', STR_PAD_LEFT) );
		
			$this->registers[$nm_reg] = $ret;	

		}
		else if( preg_match("/(BYTE|WORD|DWORD) PTR (DS|SS):\[(.*)\]/", $opcode, $match1))
		{         
			
			if ($this->debug) print_r($match1);
			
			if( preg_match("/(LOCAL)[.]([0-9]*)/", $match1[3], $match2) )
			{
				if ($this->debug) print_r( $match2 );
				
				if($this->debug) $this->show_stack();
				
				// ajustado 04/02/2016 - Validado e funcionando
				
				if( $match1[2] == "SS" )
				{   
					// SS - Stack

					$esp = $this->registers["ESP"];
					$eax = $this->registers["EAX"];
				
					$this->registers["EAX"] = $value; // Coloca valor temporario em registrador para ser usado no PUSH
			 
					reset($this->stack); // Vai para o primeiro elemento ou seja o final da pilha	     			      
					$address = key($this->stack) - 3 - (4 * base_convert( $match2[2], 10,10)); // 4 é o tamanho maximo armazenado DWORD que é 4 bytes			
					$this->registers["ESP"] = $address + 4; // Soma 4 bytes a mais pois na hora de fazer o push ele desconta 4 bytes
				
					$this->PUSH("EAX"); 

					end($this->stack);
				
					$this->registers["ESP"] = $esp; // Restaura Valores originais
					$this->registers["EAX"] = $eax; // Restaura valores originais
					
					if (!isset( $this->stack[ $address ] )) die("\r\n\r\nEnderecamento de pilha invalido. ESP = \"".base_convert( $address ,10, 16)."\" - ESP Decimal = \"{$address}\"");
             }
             else
             {
                // DS - Memory
                die("gug");
                $byte0 = $value & 0xFF;
                $byte1 = $value >> 8 & 0xFF;
                $byte2 = $value >> 16 & 0xFF;
                $byte3 = $value >> 24 & 0xFF;
                
                $address = $this->registers["EDI"];
                
                $this->memory[$address] = $byte0;
                $this->memory[$address+1] = $byte1;
                $this->memory[$address+2] = $byte2;
                $this->memory[$address+3] = $byte3;				
             }

				
				if($this->debug) $this->show_stack();
			}
			else
			{

				if (preg_match_all("/([0-9A-X]{1,8})(.)?/", $match1[3], $match2))
				{    

						$val = 0;
					
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][0]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][0] );
						else
							$val = bindec(base_convert( $match2[1][0], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
                           
                  	 $ret = $val;
						// echo "\r\n---------------------------> ESP = ".base_convert($ret, 10, 16)."\r\n"; // 
						                           
						if(isset($match2[1][1]))
						{
							if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][1]) !== False) // Verifica se eh registrador
								$val = $this->get_register( $match2[1][1] );
							else
								$val = bindec(base_convert( $match2[1][1], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
							
							$ret = $this->_inc_2_($ret, $val, "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
						}
						//echo "\r\n---------------------------> ESP = ".base_convert($ret, 10, 16)."\r\n"; // 					
						if(isset($match2[1][2]))
						{
							if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][2]) !== False) // Verifica se eh registrador
								$val = $this->get_register( $match2[1][2] );
							else
							{
								$val = bindec(base_convert( $match2[1][2], 16, 2)); // Valor hexadecimal - Converter de Hexa para Bin e depois para decimal elimina problemas de numeros negativos
								// echo "\r\n {$match2[1][2]} - {$val} \r\n";
							} 

							$ret = $this->_inc_2_($ret, $val , "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
							
						}
				
					if( $match1[2] == "SS" )
					{   
						// SS - Stack
					
						$eax = $this->get_register("EAX");  // guarda o valor original , e uso o registrador para colocar dado na pilha
						$this->set_register("EAX", $value);
				 							
						$esp = $this->get_register("ESP");
					
						$this->set_register("ESP", $ret + 4 );
				
						$this->PUSH("EAX"); // Substitui valor direto no endereço da pilha

						$this->set_register("EAX", $eax); // Restaura valor original			
						$this->set_register("ESP", $esp); // Restaura valor original					
					 }
					 else
					 {
					 	// Memoria

						// DS - Memory

						$byte0 = $value & 0xFF;
						$byte1 = $value >> 8 & 0xFF;
						$byte2 = $value >> 16 & 0xFF;
						$byte3 = $value >> 24 & 0xFF;

						$address = $ret;

						$this->memory[$address] = $byte0;
						$this->memory[$address+1] = $byte1;
						$this->memory[$address+2] = $byte2;
						$this->memory[$address+3] = $byte3;

//					 	die("bugado");
					 }

				}
				else
					die("\r\nOpcode \"{$opcode}\" invalido\r\n");				

				
			}
		}
		else if( preg_match("/\[(.*)\]/", $opcode, $match1))
		{                                             
		    
		    if (preg_match_all("/([0-9A-X]{1,8})(.)?/", $match1[1], $match2))
		    {                                                                             
                 
                 // codigo duplicado fazer ele virar uma function
                 
					$val = 0;
					
					if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][0]) !== False) // Verifica se eh registrador
						$val = $this->get_register( $match2[1][0] );
					else
						$val = base_convert( $match2[1][0], 16, 10); // Valor hexadecimal
                           
               $ret = $val;
                           
					if(isset($match2[1][1]))
					{
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][1]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][1] );
						else
							$val = base_convert( $match2[1][1], 16, 10); // Valor hexadecimal
							
						$ret = $this->_inc_2_($ret, $val, "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
					}
					
					if(isset($match2[1][2]))
					{
						if (strpos("EAX|EBX|ECX|EDX|ESP|EBP|ESI|EDI|EIP", $match2[1][2]) !== False) // Verifica se eh registrador
							$val = $this->get_register( $match2[1][2] );
						else
							$val = base_convert( $match2[1][2], 16, 10); // Valor hexadecimal
					
						$ret = $this->_inc_2_($ret, $val, "EAX"); // O terceiro parametro e passado como EAX, apenas para informar que a soma deve ser feita com 4 bytes = DWORD
					
					}

		    	//
		    }

		}		
		else
		{
			die("\r\nOpcode para acesso de memoria invalido. {$opcode}");
		}
		
		return ($ret);

	}

	function set_reserva($n){
		$this->reserva = $n;
	}
	
	function get_reserva()
	{
		return( $this->reserva); 
	}

	function set_flag_overflow($bit)
	{
		$this->flag_overflow = $bit;
	}

	function get_flag_overflow()
	{
		return( $this->flag_overflow);
	}

	function set_flag_zero($bit)
	{
		$this->flag_zero = $bit;
	}

	function get_flag_zero()
	{
		return( $this->flag_zero);
	}

	function set_flag_signal($bit)
	{
		$this->flag_signal = $bit;
	}

	function get_flag_signal()
	{
		return( $this->flag_signal);
	}
	
	function set_flag_carry($bit)
	{
		$this->flag_carry = $bit;
	}
	
	function get_flag_carry()
	{
		return( $this->flag_carry );                                                         
	}
	
	function set_flag_parity($bit)
	{
		$this->flag_parity = $bit;
	}

	function get_flag_parity()
	{
		return( $this->flag_parity );
	}

	
	function bitrev($bitstring){    
	 
		$ret = '';

		for($i=strlen($bitstring); $i >= 0; $i--)
			$ret .= $bitstring[$i];
  
  		return( $ret );
	}

	
	function show_flags()
	{
		echo " \r\n   Carry: {$this->get_flag_carry()}\r\n";
		echo "  Parity: {$this->get_flag_parity()}\r\n";
		echo "    Zero: {$this->get_flag_zero()}\r\n";
		echo "  Signal: {$this->get_flag_signal()}\r\n";
		echo "Overflow: {$this->get_flag_overflow()}\r\n";
	}	
 
	function check_parity($number)
	{
	      // Verifica se existe um número par ou ímpar de bits setados
	      
		$bin = decbin($number & 0xFF); // Paridade é feito apenas em um byte, por isto utilizado mascara
		$bin = str_pad($bin , 8, "0", STR_PAD_LEFT);
		
		$parity = "1";
		for($i = 0; $i < strlen($bin); $i++)
		    $parity   ^= $bin[$i];
		
		return( $parity );
	}

	
	function _inc_2_($a, $b, $register) // Nova versão mais otimizada
	{                                                                                    
      
      $this->set_reserva(0);
           
		$mo = $b;
		
		$i = strlen($register) - 2;

		$ro = ($i == 0)?$register[1]:$register[0];
     		
		$md = $a;

		$byte0 = $mo & 0xFF;
		$byte1 = $mo >> 8 & 0xFF;
		$byte2 = $mo >> 16 & 0xFF;
		$byte3 = $mo >> 24 & 0xFF;

		
		if( $this->debug) echo "\r\nSoma =  ". base_convert( $byte3 , 10, 16)." - ".base_convert( $byte2 , 10, 16)." - ".base_convert( $byte1 , 10, 16)." - ".base_convert( $byte0 , 10, 16);
		
		$byte0 = $md & 0xFF;
		$byte1 = $md >> 8 & 0xFF;
		$byte2 = $md >> 16 & 0xFF;
		$byte3 = $md >> 24 & 0xFF;

		if( $this->debug) echo "\r\nSoma =  ". base_convert( $byte3 , 10, 16)." - ".base_convert( $byte2 , 10, 16)." - ".base_convert( $byte1 , 10, 16)." - ".base_convert( $byte0 , 10, 16);
		
		if (strpos("LH", $ro) !== False)
		{
			if (strpos("LH", $ro) !== False)
			{
				$byte0 =  ($mo & 0xFF) + ($md & 0xFF);
				$this->set_reserva( ($byte0  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte0 = $byte0 & 0xFF;
			}
			else
			{
				$byte1 =  ($mo & 0xFF) + ($md & 0xFF);
				$this->set_reserva( ($byte0  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte1 = $byte1 & 0xFF;				
			}
		}
		if (strpos("XE", $ro) !== False)
		{
				$byte0 =  ($mo & 0xFF) + ($md & 0xFF);
				$this->set_reserva( ($byte0  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte0 = $byte0 & 0xFF;

				$byte1 =  ($mo >> 8 & 0xFF) + ($md >> 8 & 0xFF) + $this->get_reserva();				
				$this->set_reserva( ($byte1  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte1 = $byte1 & 0xFF;

		}
		if (strpos("E", $ro) !== False)
		{
				$byte2 =  ($mo >> 16 & 0xFF) + ($md >> 16 & 0xFF) + $this->get_reserva();				
				$this->set_reserva( ($byte2  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte2 = $byte2 & 0xFF;

				$byte3 =  ($mo >> 24 & 0xFF) + ($md >> 24 & 0xFF) + $this->get_reserva();				
				$this->set_reserva( ($byte3  & 0xFF00) >> 8 ); // Aplica marca 0xFF00 e descoloca 8 bits para a direita para pegar o que foi na reserva para somar na proxima interacao
				$byte3 = $byte3 & 0xFF;
		}

		if( $this->debug) echo "\r\nSoma =  ". base_convert( $byte3 , 10, 16)." - ".base_convert( $byte2 , 10, 16)." - ".base_convert( $byte1 , 10, 16)." - ".base_convert( $byte0 , 10, 16);

		$ret = bindec( str_pad( decbin($byte3) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte2) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte1) ,8, '0', STR_PAD_LEFT).str_pad( decbin($byte0) ,8, '0', STR_PAD_LEFT) );
		
		return($ret);
	}	

      /**************** funcoes de apoio *************/

	function PUSH_MEMORY($mem)
	{    
		for($i=0; $i < strlen($mem); $i++)
			$this->memory[ $this->registers["EDI"] + $i ] = ord( $mem[$i] );
	}
	
	function PUSH_STACK($mem)
	{    
		
		if($this->debug) echo "\r\n\r\nN Bytes para a pilha: ".strlen($mem)."\r\n\r\n";
	
		for($deslocamento=strlen($mem); $deslocamento > 0; $deslocamento-=4)
		{ 
			$byte3 = decbin(ord($mem[$deslocamento-1]));
			$byte2 = decbin(ord($mem[$deslocamento-2]));			
			$byte1 = decbin(ord($mem[$deslocamento-3]));
			$byte0 = decbin(ord($mem[$deslocamento-4]));

	
			if($this->debug) echo " OR  = ".str_pad( $byte3 ,8, '0', STR_PAD_LEFT)."--".str_pad( $byte2 ,8, '0', STR_PAD_LEFT)."--".str_pad( $byte1 ,8, '0', STR_PAD_LEFT)."--".str_pad( $byte0 ,8, '0', STR_PAD_LEFT)."\r\n";	
	
			$ret = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );
         
         //echo "\r\n".base_convert($ret, 10, 16)."\r\n"; // apagar
      
			$this->PUSH($ret);
		}
	
	}
      
      /******************************* intrucoes assembly ******************************/
	function NOP()
	{ 
		// Nao faz nada
	}
	
	function CLC()
	{
		$this->set_flag_carry(0);
	}
	
	function CMC()
	{
		if($this->get_flag_carry())
		   $this->set_flag_carry(0);
		else
			$this->set_flag_carry(1);
	}                           
	
	function CMP($dest, $src)
	{

			if ( strpos("LH", $dest['1']) !== False ) 
			{
				$overflow = 0x80;
				$bitmask = 0xFF;
			} 
			else if ( strpos("X", $dest['1']) !== False ) 
			{
				$overflow = 0x8000;
				$bitmask= 0xFFFF;		
			}
			else if ( strpos("E", $dest['0']) !== False ) 
			{
				$overflow = 0x80000000;
				$bitmask= 0xFFFFFFFF;		
			}
			
			$dest1 = $dest;
			if(is_string($dest))
				$dest1 = $this->get_register($dest); // Se for Registrador pega valor		
         
			$src1 = $src;
			if(is_string($src))
				$src1 = $this->get_register($src); // Se for Registrador pega valor			
			
			$resultado = $this->_inc_2_( $dest1 , 0 - $src1 , $dest );	
			
			echo "\r\nResultado = {$resultado} === {$dest1} + ".(0 - $src1)."\r\n";					

         // Seta carry, ocorreu um vai um na reserva        
         if( ($dest1 - $src1) < 0 )
				$this->set_flag_carry(1);
			else
				$this->set_flag_carry(0);				

			// Seta flag Overflow      
         if((($dest1 & $overflow) == 0) && ($src1 & $overflow) && ($resultado & $overflow))
         {
         	// 1. If the sum of two numbers with the sign bits off yields a result number with the sign bit on, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else if(($dest1 & $overflow) && (($src1 & $overflow) == 0) && (($resultado & $overflow) == 0))
         {
         	// 2. If the sum of two numbers with the sign bits on yields a result number with the sign bit off, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else
         {
         	// Otherwise, the overflow flag is turned off.
         	$this->set_flag_overflow(0);         
         }
         
			// Seta flag Paridade			
			if ($this->check_parity($resultado)) $this->set_flag_parity(1); else $this->set_flag_parity(0);

			// Seta flag Sinal
			if($resultado & $overflow) $this->set_flag_signal(1); else $this->set_flag_signal(0);
			
			// Seta flag Zero	
			if( $resultado == 0) $this->set_flag_zero(1); else $this->set_flag_zero(0);
				
	}

	function MOV($dest, $orig)
	{	
		
		if(is_string($orig))
			$orig = $this->get_register($orig); // Se for Registrador pega valor
		
		$this->set_register($dest, $orig);
	}

	function POP($dest)
	{	
		// Retira dados da pilha
		
		end($this->stack); // vai para o topo da Pilha, que é o ultimo elemento que entrou no array
		
		$byte0 = decbin( array_pop($this->stack));
		$byte1 = decbin(array_pop($this->stack));
		$byte2 = decbin(array_pop($this->stack));
		$byte3 = decbin(array_pop($this->stack));

		$ret = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );

		if($this->debug) echo "\r\nPOP1 : ". base_convert($ret, 10, 16)."\r\n";
		
		$this->set_register("ESP", $this->get_register("ESP") + 4 ); // Atualiza o ultimo endereço da pilha usado
				
		$this->set_register($dest, $ret  ); // Registrador de destino que vai receber o valor armazena na pilha	
	}

		
	function PUSH($orig)
	{  
		// Insere dados na Pilha  
		
		$esp =  $this->get_register("ESP");
				
		if($this->debug) echo "\r\nESP: {$esp}\r\n";

		$i = strlen($orig) - 2;                 

		if(is_string($orig))
		{	
			if(!isset($this->registers[$orig])) die("\r\nPUSH aceita apenas registradores de 4bytes (DWORD)");
			$value = $this->get_register($orig); // Obtem dados do registrador
		}
		else
			$value = $orig;

		$esp = $this->get_register("ESP") - 4;
		
		$base = $esp + 3;
		
		$byte0 = $value & 0xFF;
		$byte1 = $value >> 8 & 0xFF;
		$byte2 = $value >> 16 & 0xFF;
		$byte3 = $value >> 24 & 0xFF;
		
		$this->stack[$base] = $byte3;
		$this->stack[$base-1] = $byte2;		
		$this->stack[$base-2] = $byte1;		
		$this->stack[$base-3] = $byte0;

		if($this->debug) print_r($this->stack);
			
		$this->set_register("ESP", $base-3 ); // Atualiza o ultimo endereço da pilha usado 		
	}
      

      
	function ADD($dest, $orig)
	{
         // Flags validadas 12/02/2016 - 15h
         // http://teaching.idallen.com/dat2343/10f/notes/040_overflow.txt

			if ( strpos("LH", $dest['1']) !== False ) 
			{
				$overflow = 0x80;
				$bitmask = 0xFF;
			} 
			else if ( strpos("X", $dest['1']) !== False ) 
			{
				$overflow = 0x8000;
				$bitmask= 0xFFFF;		
			}
			else if ( strpos("E", $dest['0']) !== False ) 
			{
				$overflow = 0x80000000;
				$bitmask= 0xFFFFFFFF;		
			}
		                     
			// Invoca função de soma chamada _inc_2 , a variavel bitmask tem a função de eliminar bits que não ultrpassam o limite de bits esperado para o registrador que vai receber o dado
			
			//echo "\r\n--->dest = {$dest}  -- orig={$orig} ";
			
			$valorAntDest = $this->get_register($dest);
	
			if(is_string($orig))
				$orig = $this->get_register($orig); // Se for Registrador pega valor				
			
			$resultado = $this->_inc_2_( $valorAntDest , $orig  , $dest );		

			
			$this->set_register($dest , $resultado & $bitmask);  // Armazena Resultado do Calculo
                     
         //$this->debug = 1;
			if($this->debug) echo "\r\nadd({$dest},{$orig})::: ".base_convert( $orig ,10,16)."\r\n";
			//$this->debug = 0;
            

         // Se estourou o limite máximo permitido do registrador de destino, ocorreu um vai um na reserva        
         if( $this->get_reserva())
				$this->set_flag_carry(1);
			else
				$this->set_flag_carry(0);				

			// Seta flag Overflow      
         if((($valorAntDest & $overflow) == 0) && (($orig & $overflow) == 0) && ($resultado & $overflow))
         {
         	// 1. If the sum of two numbers with the sign bits off yields a result number with the sign bit on, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else if(($valorAntDest & $overflow) && ($orig & $overflow) && (($resultado & $overflow) == 0))
         {
         	// 2. If the sum of two numbers with the sign bits on yields a result number with the sign bit off, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else
         {
         	// Otherwise, the overflow flag is turned off.
         	$this->set_flag_overflow(0);         
         }
         
			// Seta flag Paridade			
			if ($this->check_parity($resultado)) $this->set_flag_parity(1); else $this->set_flag_parity(0);

			// Seta flag Sinal
			if($resultado & $overflow) $this->set_flag_signal(1); else $this->set_flag_signal(0);
			
			// Seta flag Zero	
			if( $resultado == 0) $this->set_flag_zero(1); else $this->set_flag_zero(0);
					
			return($resultado & $bitmask);		
	}                                

	function SUB($dest, $orig)
	{
	      // FALTA VALIDAR TODA A FUNCAO
	      // COM FLAGS
          
			if ( strpos("LH", $dest['1']) !== False ) 
			{
				$overflow = 0x80;
				$bitmask = 0xFF;
			} 
			else if ( strpos("X", $dest['1']) !== False ) 
			{
				$overflow = 0x8000;
				$bitmask= 0xFFFF;		
			}
			else if ( strpos("E", $dest['0']) !== False ) 
			{
				$overflow = 0x80000000;
				$bitmask= 0xFFFFFFFF;		
			}


			$valorAntDest = $this->get_register($dest);
	
			if(is_string($orig))
				$orig = $this->get_register($orig); // Se for Registrador pega valor				
			
			$resultado = $this->_inc_2_( $valorAntDest , 0 - $orig  , $dest );		

			$this->set_register($dest , $resultado & $bitmask);  // Armazena Resultado do Calculo	
			                     

			if($this->debug) echo "\r\nsub({$dest},{$orig})::: ".base_convert( $this->get_register($dest) ,10,16)."\r\n";


         // Seta Carry
         if( $valorAntDest - $orig < 0 )
				$this->set_flag_carry(1);
			else
				$this->set_flag_carry(0);	
				
			// Seta flag Overflow      
         if( (($valorAntDest & $overflow) == 0) && ($orig & $overflow) && ($resultado & $overflow) )
         {
            // Valor positivo e valor negativo e o resultado for positivo
         	// 1. If the sum of two numbers with the sign bits off yields a result number with the sign bit on, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else if( ($valorAntDest & $overflow) && (($orig & $overflow) == 0) && (($resultado & $overflow) == 0))
         {
         	// valor negativo e valor positivo e o resultado
         	// 2. If the sum of two numbers with the sign bits on yields a result number with the sign bit off, the "overflow" flag is turned on.
         	$this->set_flag_overflow(1);
         }
         else
         {
         	// Otherwise, the overflow flag is turned off.
         	$this->set_flag_overflow(0);         
         }				

			// Seta flag Paridade			
			if ($this->check_parity($resultado)) $this->set_flag_parity(1); else $this->set_flag_parity(0);

			// Seta flag Sinal
			if($resultado & $overflow) $this->set_flag_signal(1); else $this->set_flag_signal(0);
			
			// Seta flag Zero	
			if( $resultado == 0) $this->set_flag_zero(1); else $this->set_flag_zero(0);
						
	}                                

	function INC($dest)
	{
         // Adds one to destination unsigned binary operand.
	      // Flags Afetadas pelo incremento: AF OF PF SF ZF 
	      // Flag "A"  falta implementar

           $this->set_flag_carry(0); // Incremento não gera Carry

		if ( ( strpos("LHX", $dest['1']) !== False ) || ( strpos("E", $dest['0']) !== False ) )
		{
//			$this->getMASK($dest, $overflow, $bitmask); // Carrega mascara de bits para overflow e bitmask de acordo com o tipo do registrador
		}
		else
		{
			die("\r\nOperador \"{$dest}\" desonconhecido");
		}
		

		if ( strpos("LH", $dest['1']) !== False )
		{
			$overflow = 0x80;
			$bitmask = 0xFF;
		} 
		else if ( strpos("X", $dest['1']) !== False )
		{
			$overflow = 0x8000;
			$bitmask= 0xFFFF;		
		}
		else if ( strpos("E", $dest['0']) !== False )
		{
			$overflow = 0x80000000;
			$bitmask= 0xFFFFFFFF;		
		}
		else
		{
			die("\r\nOperador \"{$dest}\" desonconhecido");
		}

		                     
           // Invoca função de soma chamada _inc_2 , a variavel bitmask tem a função de eliminar bits que não ultrpassam o limite de bits esperado para o registrador que vai receber o dado
           
           $this->set_register($dest , $this->_inc_2_( $this->get_register($dest) , 0x01  , $dest ) & $bitmask);  // Armazena Resultado do Calculo
                     

		if($this->debug) echo "\r\nINC({$dest})::: ".base_convert( $this->get_register($dest) ,10,16)."\r\n";
				
		           
           if ($this->get_reserva()) // se houve estouro o resto foi para o proximo byte que eh a reserva 
           	$this->set_flag_overflow(1);
           else if( $this->get_register($dest) == $overflow) 
                $this->set_flag_overflow(1) ;
           else $this->set_flag_overflow(0); // Se a sinalizacao anteriro ficou o contrario é porque houve estouro, ou o numero passou de positivo para negativo ou vice versa.
           
           
           if ($this->check_parity($byte & $bitmask)) $this->set_flag_parity(1); else $this->set_flag_parity(0);

		if($byte & $overflow) $this->set_flag_signal(1); else $this->set_flag_signal(0);
				
		if( ($byte & $bitmask) == 0) $this->set_flag_zero(1); else $this->set_flag_zero(0);
					
		return($byte & $bitmask);		
	}            

	function getMASK($dest, & $overflow , & $bitmask)
	{
		if ( strpos("LH", $dest['1']) !== False )
		{
			$overflow = 0x80;
			$bitmask = 0xFF;
		} 
		else if ( strpos("X", $dest['1']) !== False )
		{
			$overflow = 0x8000;
			$bitmask= 0xFFFF;		
		}
		else if ( strpos("E", $dest['0']) !== False )
		{
			$overflow = 0x80000000;
			$bitmask= 0xFFFFFFFF;	
		}
	}

	function BITAND($dest, $orig)
	{

	     // A função precisou ser criada pois fazer um bit binário de um valor de mais de 31bits da problema.
	     // O sistema trabalha como número negativo.
	     // Para trabalhar com 32 bits , sem sinal foi criado esta função.
         
		$abin  =  str_pad( base_convert( $this->get_register($dest) ,10,2) , 32, '0', STR_PAD_LEFT);
		$bbin  =  str_pad( base_convert(  $this->get_register($orig) ,10,2) , 32, '0', STR_PAD_LEFT);
	
		$abin3 = base_convert( substr($abin,0,8) , 2, 2);
		$abin2 = base_convert( substr($abin,8,8), 2, 2);
		$abin1 = base_convert( substr($abin,16,8) , 2, 2);
		$abin0 = base_convert( substr($abin,24), 2, 2);

		$bbin3 = base_convert( substr($bbin,0,8) , 2, 2);
		$bbin2 = base_convert( substr($bbin,8,8), 2, 2);
		$bbin1 = base_convert( substr($bbin,16,8) , 2, 2);
		$bbin0 = base_convert( substr($bbin,24), 2, 2);
	
		$byte3 = decbin(bindec($abin3) & bindec($bbin3));
		$byte2 = decbin(bindec($abin2) & bindec($bbin2));
		$byte1 = decbin(bindec($abin1) & bindec($bbin1));
		$byte0 = decbin(bindec($abin0) & bindec($bbin0));
	
		$value = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );
		
		$this->set_register($dest , $value);
		
		$this->getMASK($dest, $overflow, $bitmask); // Carrega mascaras de bits para overflow e bitmask de acordo com o tipo do registrador
           
		$this->set_flag_carry(0);        // AND não gera Carry
		$this->set_flag_overflow(0);  // AND não gera Overflow
	     
		if ($this->check_parity($this->get_register($dest) & $bitmask)) $this->set_flag_parity(1); else $this->set_flag_parity(0);
		 
	}	      	

	function BITOR($dest, $orig)
	{

	     // A função precisou ser criada pois fazer um bit binário de um valor de mais de 31bits da problema.
	     // O sistema trabalha como número negativo.
	     // Para trabalhar com 32 bits , sem sinal foi criado esta função.
         
		$abin  =  str_pad( base_convert( $this->get_register($dest) ,10,2) , 32, '0', STR_PAD_LEFT);
		$bbin  =  str_pad( base_convert(  $this->get_register($orig) ,10,2) , 32, '0', STR_PAD_LEFT);
	
		$abin3 = base_convert( substr($abin,0,8) , 2, 2);
		$abin2 = base_convert( substr($abin,8,8), 2, 2);
		$abin1 = base_convert( substr($abin,16,8) , 2, 2);
		$abin0 = base_convert( substr($abin,24), 2, 2);

		$bbin3 = base_convert( substr($bbin,0,8) , 2, 2);
		$bbin2 = base_convert( substr($bbin,8,8), 2, 2);
		$bbin1 = base_convert( substr($bbin,16,8) , 2, 2);
		$bbin0 = base_convert( substr($bbin,24), 2, 2);
	
		$byte3 = decbin(bindec($abin3) | bindec($bbin3));
		$byte2 = decbin(bindec($abin2) | bindec($bbin2));
		$byte1 = decbin(bindec($abin1) | bindec($bbin1));
		$byte0 = decbin(bindec($abin0) | bindec($bbin0));
	
		$value = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );
		
		$this->set_register($dest , $value);
		
		$this->getMASK($dest, $overflow, $bitmask); // Carrega mascaras de bits para overflow e bitmask de acordo com o tipo do registrador
           
		$this->set_flag_carry(0);        // AND não gera Carry
		$this->set_flag_overflow(0);  // AND não gera Overflow
	     
		if ($this->check_parity($this->get_register($dest) & $bitmask)) $this->set_flag_parity(1); else $this->set_flag_parity(0);
		 
	}	    
	
	function BITXOR($dest, $orig)
	{

	     // A função precisou ser criada, pois fazer um bit binário de um valor de mais de 31bits da problema.
	     // O sistema trabalha como número negativo.
	     // Para trabalhar com 32 bits , sem sinal foi criado esta função.
         
		$abin  =  str_pad( base_convert( $this->get_register($dest) ,10,2) , 32, '0', STR_PAD_LEFT);
		$bbin  =  str_pad( base_convert(  $this->get_register($orig) ,10,2) , 32, '0', STR_PAD_LEFT);
	
		$abin3 = base_convert( substr($abin,0,8) , 2, 2);
		$abin2 = base_convert( substr($abin,8,8), 2, 2);
		$abin1 = base_convert( substr($abin,16,8) , 2, 2);
		$abin0 = base_convert( substr($abin,24), 2, 2);

		$bbin3 = base_convert( substr($bbin,0,8) , 2, 2);
		$bbin2 = base_convert( substr($bbin,8,8), 2, 2);
		$bbin1 = base_convert( substr($bbin,16,8) , 2, 2);
		$bbin0 = base_convert( substr($bbin,24), 2, 2);
	
		$byte3 = decbin(bindec($abin3) ^ bindec($bbin3));
		$byte2 = decbin(bindec($abin2) ^ bindec($bbin2));
		$byte1 = decbin(bindec($abin1) ^ bindec($bbin1));
		$byte0 = decbin(bindec($abin0) ^ bindec($bbin0));
	
		$value = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );
		
		$this->set_register($dest , $value);
		
		$this->getMASK($dest, $overflow, $bitmask); // Carrega mascaras de bits para overflow e bitmask de acordo com o tipo do registrador
           
		$this->set_flag_carry(0);        // AND não gera Carry
		$this->set_flag_overflow(0);  // AND não gera Overflow
	     
		if ($this->check_parity($this->get_register($dest) & $bitmask)) $this->set_flag_parity(1); else $this->set_flag_parity(0);
		 
	}
	
	function TEST( $source1 , $source2 )
	{

		// Ao fazer um bit binário de um valor de mais de 31bits da problema.
		// Para manipular os 32 bits sem levar em conta o sinal, foi criada esta abordagem.
         
		$abin  =  str_pad( base_convert( $this->get_register($source1) ,10,2) , 32, '0', STR_PAD_LEFT);
		$bbin  =  str_pad( base_convert( $this->get_register($source2) ,10,2) , 32, '0', STR_PAD_LEFT);
	
		$abin3 = base_convert( substr($abin,0,8) , 2, 2);
		$abin2 = base_convert( substr($abin,8,8), 2, 2);
		$abin1 = base_convert( substr($abin,16,8) , 2, 2);
		$abin0 = base_convert( substr($abin,24), 2, 2);

		$bbin3 = base_convert( substr($bbin,0,8) , 2, 2);
		$bbin2 = base_convert( substr($bbin,8,8), 2, 2);
		$bbin1 = base_convert( substr($bbin,16,8) , 2, 2);
		$bbin0 = base_convert( substr($bbin,24), 2, 2);
	
		$byte3 = decbin(bindec($abin3) & bindec($bbin3));
		$byte2 = decbin(bindec($abin2) & bindec($bbin2));
		$byte1 = decbin(bindec($abin1) & bindec($bbin1));
		$byte0 = decbin(bindec($abin0) & bindec($bbin0));
	
		$value = bindec( str_pad( $byte3 ,8, '0', STR_PAD_LEFT).str_pad( $byte2 ,8, '0', STR_PAD_LEFT).str_pad( $byte1 ,8, '0', STR_PAD_LEFT).str_pad( $byte0 ,8, '0', STR_PAD_LEFT) );
                
                echo "{$value}\r\n";
                
                // fim da abordagem
                
                $this->getMASK($source1, $mascara1, $bitmask1); // Carrega mascaras de bits para overflow e bitmask de acordo com o tipo do registrador
                $this->getMASK($source2, $mascara2, $bitmask2); // Carrega mascaras de bits para overflow e bitmask de acordo com o tipo do registrador
		                
                if($mascara1 <> $mascara2) 
                  die("ATENÇÃO: A quantidade de bits de Source1 e Source2 são diferentes.");
                
		// Obtem o bit mais significativo
      		$MSB = ( $value & $mascara1) ? 1 : 0 ;      		
      		$this->set_flag_signal( $MSB );
		
		$paridade = ($this->check_parity( $value ) == "0") ? 0 : 1;		
		$this->set_flag_parity( $paridade );	
				
		$this->set_flag_carry(0);
		$this->set_flag_overflow(0);
		// AF = Undefined  // Criar flag no futuro
	}

	function NOT($dest)
	{
           // Complemento de 1
           // Não mofifica nenhum Flag
           // Status: 100% funcional
           
		$i = strlen($dest) - 2;
			
		$abin  =  str_pad( base_convert( $this->registers["E".$dest[$i]."X"] ,10,2) , 32, '0', STR_PAD_LEFT);	
				
    		$arr = str_split($abin);  

    		$abin = "";
    		foreach($arr as $i)
      		$abin .= ($i == "0") ? "1" : "0";
		
		$binario = "";

           if ( strpos("HX", $dest['1']) !== False )
			$binario .= substr($abin,16,8);		 
           if ( strpos("LX", $dest['1']) !== False )
			$binario .= substr($abin,24,8);
		if ( strpos("E", $dest['0']) !== False )
		{
			$binario .= substr($abin,0,8);		 
			$binario .= substr($abin,8,8);		 
			$binario .= substr($abin,16,8);		 
			$binario .= substr($abin,24,8);		 
		}
           
		$this->set_register($dest , bindec($binario) );

	}
	
	function NEG($dest)
	{
      // Complemento de 2
      // Pode ser feito de uma forma muito simples subrai de 0 o dest. Exemplo: x = 0 - 118 (0x76)  ==> x= -118 (0x8A)
      // Fazer uma análise de otimização de codigo para o simulador voar
          
		$this->NOT($dest); // Inverte todos os bits fazendo complemento de 1
     	$this->INC($dest);
	}	

	function ROL($dest, $bits)
	{
		// Rotação de Bits, equivalente ao rol do assembly. Dword 32 bits.
		
		// fazer tratamento para L, H, X = 8 BITS, 16bits, os 32 bits ja esta ok
           
		if ( strpos("LH", $dest['1']) !== False )
			$numbits = 8;
		else if ( strpos("X", $dest['1']) !== False )
			$numbits = 16;
		else if ( strpos("E", $dest['0']) !== False )
			$numbits = 32;         
						
		$abin  =  str_pad( base_convert( $this->get_register($dest) ,10,2) , $numbits, '0', STR_PAD_LEFT);
		
		//echo "\r\n BINARIO : ".$abin."\r\n\r\n";
		
    	$value = bindec(substr($abin, $bits).substr($abin, 0, $bits)) ;
    				
		$this->set_register($dest , $value );
	}

	function ROR($dest, $bits)
	{
		// Rotação de Bits, equivalente ao ror do assembly. Dword 32 bits.

		// fazer tratamento para L, H, X = 8 BITS, 16bits, os 32 bits ja esta ok
           
		if ( strpos("LH", $dest['1']) !== False )
			$numbits = 8;
		else if ( strpos("X", $dest['1']) !== False )
			$numbits = 16;
		else if ( strpos("E", $dest['0']) !== False )
			$numbits = 32;         
							
		$abin  =  str_pad( base_convert( $this->get_register($dest) ,10,2) , $numbits, '0', STR_PAD_LEFT);
		
		if ($debug) echo "\r\n BINARIO : ".$abin."\r\n\r\n";
		if ($debug) echo "\r\n     ROR : ".substr($abin, $numbits - $bits).substr($abin, 0, $numbits - $bits)."\r\n\r\n";
		
    	$value = bindec(substr($abin, $numbits - $bits).substr($abin, 0, $numbits - $bits)) ;
    				
		$this->set_register($dest , $value );           
	}
	
	function SHR ($dest, $orig)
	{
		if(is_string($orig))  // Se for Registrador pega valor
			$orig = $this->get_register($orig);
		
		$this->set_flag_carry(( $this->get_register($dest) >> ($orig - 1)) & 0x01); // O ultimo bit que for deslocado apos o bit menos significativo vai ser atribuido ao carry
		
		$this->set_register($dest , ( $this->get_register($dest) >> $orig) );
		
	}

	function SHL ($dest, $orig)
	{   
	
		// Carry validado
		// 
		if(is_string($orig))  // Se for Registrador pega valor
			$orig = $this->get_register($orig);

		$nbits = $orig;

		$a = $this->get_register($dest);

           if ( strpos("LH", $dest['1']) !== False )
			$numbits = 8;
		else if ( strpos("X", $dest['1']) !== False )
			$numbits = 16;
		else if ( strpos("E", $dest['0']) !== False )
			$numbits = 32;
					         		
		$abin  =  str_pad( base_convert( $a ,10,2) , $numbits, '0', STR_PAD_LEFT);
		
		$this->getMASK($dest, $overflow, $bitmask);
		
    		           
		echo "\r\n\r\r#{$abin}#\r\n$". str_pad("^", $nbits, ' ', STR_PAD_LEFT)."\r\n";
		echo "\r\r>>>>>> R : ".$abin[$nbits-1]."\r\n";
		echo "\r\nX: ".  base_convert( ($a << $nbits) & $bitmask, 10, 16);
			
		
		$this->set_flag_carry( $abin[$nbits-1] ); // O ultimo bit que for desolocado apos o bit menos significativo vai ser atribuido ao carry
		
		$ret = ($a << $nbits) & $bitmask;
		
		$this->set_register($dest , $ret );
		
	}
	
	function LEA ($dest, $orig)
	{
		$this->set_register($dest, $this->get_register($orig) );
	}
		  	
		

}	

?>