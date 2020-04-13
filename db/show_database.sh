user=root
password="123456"
db="hw-mto10-u1752877"

show_student="call get_student(null);";
show_gameinfo_stest="select * from gameinfo_stest limit 10;";
mysql -u${user} -p${password} $db -e "$show_student";
mysql -u${user} -p${password} $db -e "$show_gameinfo_stest";