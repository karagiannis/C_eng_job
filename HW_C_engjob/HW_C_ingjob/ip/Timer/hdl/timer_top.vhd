library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity timer_top is
port (
		Control_timer:	in std_logic_vector(1 downto 0);
		clk:				in std_logic;
		reset_n:			in std_logic;
		timer_data:		inout std_logic_vector(31 downto 0)
		);
end timer_top;

architecture timer_top_rtl of timer_top is
--signal timer_reg: std_logic_vector(31 downto 0);

begin
	HW_function_process: process(clk,reset_n)
	begin
		if reset_n ='0' then
			timer_data<= (others => '0');
		elsif rising_edge(clk) then
			if Control_timer = "10" then
				timer_data<=1 + timer_data;
			elsif Control_timer = "00" then
				timer_data<=timer_data;
			elsif Control_timer = "01" then
				timer_data<=(others => '0');
			else
				null;
			end if;
		else
			null;
		end if;
	end process HW_function_process;
end timer_top_rtl;