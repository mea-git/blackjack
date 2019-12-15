#include <iostream>
#include <time.h>

char suit_symbols[4] = { '\x6', '\x5', '\x4', '\x3' };

struct card_t
{
	char symbol[3];
	int points;
};

struct card_t cards[13] =
{
	{ "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 },
	{ "10", 10 }, { "J", 10 }, { "Q", 10 }, { "K", 10 }, { "A", 11 }
};

void card_print(int card_index)
{
	int card_suit = card_index / 13;
	int card_no = card_index % 13;
	printf("%s%c", cards[card_no].symbol, suit_symbols[card_suit]);
}

struct shoe_t
{
	int cards[52];
	int cards_count;//����� ���������� ����
};

struct shoe_t shoe;
 
void shoe_init(int pack_count) //��������� ������� 
{
	for (int i = 0; i < 52; i++)
		shoe.cards[i] = pack_count;
	shoe.cards_count = 52 * pack_count;
	srand(time(NULL));
}

int shoe_pick_card() //����������� ��������� �����
{
	if (shoe.cards_count == 0)
		return -1; // ������ ������
	for ( ; ; )
	{
		int i = rand() % 52;
		if (shoe.cards[i])
		{
			shoe.cards[i]--;
			shoe.cards_count--;
			return i;
		}
	}
}

typedef struct hand_t
{
	int cards[5];
	int cards_count;
} hand_t;

hand_t gambler, dealer;

void clear_hands() //������� ���
{
	for (int i = 0; i < 5; i++)
	{
		gambler.cards[i] = -1;
		dealer.cards[i] = -1;
	}
	gambler.cards_count = 0;
	dealer.cards_count = 0;
}

int gambler_pick_card()
{
	if (gambler.cards_count == 5)
	{
		printf("� ������ �� ���� ��� 5 ����\n");
		return -1;
	}
	int card = shoe_pick_card();
	if (card == -1)
	{
		printf("������ ��� �����\n");
		return -2;
	}
	gambler.cards[gambler.cards_count] = card;
	gambler.cards_count++;
}

int dealer_pick_card()
{
	if (dealer.cards_count == 5)
	{
		printf("� ������ �� ���� ��� 5 ����\n");
		return -1;
	}
	int card = shoe_pick_card();
	if (card == -1)
	{
		printf("������ ��� �����\n");
		return -2;
	}
	dealer.cards[dealer.cards_count] = card;
	dealer.cards_count++;
}

int hand_pick_card(hand_t* p_hand)
{

}

void hand_print_cards(struct hand_t hand)
{
	for (int i = 0; i < hand.cards_count; i++)
	{
		card_print(hand.cards[i]);
		printf(" ");
	}
}

int hand_points(struct hand_t hand) // ������� ���������� ����� �� ����
{
	int points = 0; // ���������� �����
	int aces = 0; // ���������� �����
	int card_no; // ����� ����� � ������
	for (int i = 0; i < hand.cards_count; i++)
	{
		card_no = hand.cards[i] % 13;
		points += cards[card_no].points;
		if (card_no == 12)
			aces++;
	}
	for (; (points > 21) && (aces > 0); ) // ���� �������, �� ������� ���� �� 1 ����, � �� �� 11
	{
		points -= 10;
		aces--;
	}
	return points;
}


void shoe_print() // ��� ������� �������� ������
{
	int cards_count = shoe.cards_count;
	for (int i = 0; i < cards_count; i++)
	{
		card_print(shoe_pick_card());
		printf(" ");
	}
}



int main()
{
	//system("chcp 1251");
	setlocale(LC_ALL, "rus");

	shoe_init(1);

	// ������� ����
	clear_hands();
	
	// ������� ��� ����� ������
	gambler_pick_card();
	gambler_pick_card();
	printf("����� ������: ");
	hand_print_cards(gambler);
	printf("\n");
	printf("���� ������: %d\n", hand_points(gambler));

	// ������� ��� ����� ������
	dealer_pick_card();
	dealer_pick_card();
	printf("����� ������: ");
	hand_print_cards(dealer);
	printf("\n");
	printf("���� ������: %d\n", hand_points(dealer));

	char answer;
	int gambler_points = hand_points(gambler);
	do
	{
		printf("������ ����� �����? (y/n)\n");
		scanf_s(" %c", &answer, 1);
		if (answer == 'y')
		{
			int result = gambler_pick_card();
			if (result == -1) // � ������ ��� ���� 5 ����
			{
				break;
			}
			if (result == -2) // � ���� ��� ��� ����
			{
				printf("� ���� ��� ��� ����! \n");
				break;
			}
			printf("����� ������: ");
			hand_print_cards(gambler);
			printf("\n");
			gambler_points = hand_points(gambler);
			printf("���� ������: %d\n", gambler_points);
			if (gambler_points > 21) // ������� � ������
			{
				printf("� ������ �������! \n");
				break;
			}
		}
		
	}
	while (answer == 'y');

	if (gambler_points > 21) // ���� ������� � ������, �� ������ ��������
	{
		printf("\n�������� ������! \n");
		return 0;
	}

	int dealer_points = hand_points(dealer);
	while (dealer_points < 17) // ���� ������, �� ����� ������� ����� �����
	{
		printf("����� ����� ���! \n");
		dealer_pick_card();
		printf("����� ������: ");
		hand_print_cards(dealer);
		printf("\n");
		dealer_points = hand_points(dealer);
		printf("���� ������: %d\n", dealer_points);
	}

	if (dealer_points > 21) // ���� ������� � ������, �� ������ ��������
	{
		printf("\n� ������ �������. ������� �����! \n");
		return 0;
	}

	if (dealer_points > gambler_points) // � ������ ������, ��� � ������
	{
		printf("\n� ������ ������ �����. �������� ������! \n");
		return 0;
	}
	else
	{
		printf("\n� ������ ������ �����. ������� �����! \n");
		return 0;
	}

	printf("������: ");
	shoe_print();
	printf("\n");

	system("pause");
	return EXIT_SUCCESS;
}