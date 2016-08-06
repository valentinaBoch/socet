#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h> //константы и функции для работы с сокетами

#define SOCK_NAME "socket.soc"
#define BUF_SIZE 256

int main(int argc, char ** argv)
{
	struct sockaddr srvr_name, rcvr_name;
	char buf[BUF_SIZE];
	int   sock;
	int   namelen, bytes;

	sock = socket(AF_UNIX, SOCK_DGRAM, 0); //команда для получения дескриптора сокета, 1-ый параметр - домен, к которому принадлежит сокет (обозначает тип соединения). 2-ой - тип сокета, 3-ий - протокол ПД. в случае ошибки - возвращает "-1"
	if (sock < 0) 
	{
		perror("socket failed");
		return EXIT_FAILURE;
	}
	srvr_name.sa_family = AF_UNIX;
	strcpy(srvr_name.sa_data, SOCK_NAME);
	// после получения дескриптора вызываем ф-ю bind

/*
	Вместо общей структуры sockaddr для сокетов Unix (сокетов в файловом пространстве имен) можно использовать специализированную структуру sockaddr_un. 
	Поле sockaddr.sa_family позволяет указать семейство адресов, которым мы будем пользоваться. 
	В нашем случае это семейство адресов файловых сокетов Unix AF_UNIX. 
	Сам адрес семейства AF_UNIX (поле sa_data) представляет собой обычное имя файла сокета. 
	После вызова bind() наша программа-сервер становится доступна для соединения по заданному адресу (имени файла).
*/

	if (bind(sock, &srvr_name, strlen(srvr_name.sa_data) + // связывает сокет с заданным адресом ..
        	sizeof(srvr_name.sa_family)) < 0) 
	{
		perror("bind failed");
		return EXIT_FAILURE;
	}

	bytes = recvfrom(sock, buf, sizeof(buf),  0, &rcvr_name, &namelen); //При обмене данными с датаграммными сокетами мы используем не функции write() и read(), а специальные функции recvfrom(2) и sendto(2).  Для чтения данных из датаграммного сокета мы используем функцию recvfrom(2), которая по умолчанию блокирует программу до тех пор, пока на входе не появятся новые данные.
	if (bytes < 0) 
	{
		perror("recvfrom failed");
    		return EXIT_FAILURE;
  	}
	buf[bytes] = 0;
	rcvr_name.sa_data[namelen] = 0;
	printf("Client sent: %s\n", buf);
	close(sock);  	   //закрытие сокета
	unlink(SOCK_NAME); //удаление созданного сокета
}
 

