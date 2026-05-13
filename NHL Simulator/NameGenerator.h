#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include "FullName.h"

struct Names{
	std::vector<std::string> firstNames; //all male names for this nationality
	std::vector<std::string> lastNames; //all last names for this nationality
};

static class NameGenerator {
public:
	std::unordered_map<std::string, Names> nameByNationality;

	void loadNames() {
		nameByNationality["American"] = {
			.firstNames = {
				"Liam", "Noah", "Oliver", "James", "Elijah","William", "Benjamin", "Lucas", "Henry", "Alexander",
				"Ethan", "Michael", "Daniel", "Jacob", "Logan","Jackson", "Levi", "Sebastian", "Mateo", "Jack",
				"Owen", "Theodore", "Aiden", "Samuel", "Joseph","David", "John", "Luke", "Isaac", "Gabriel",
				"Anthony", "Dylan", "Carter", "Thomas", "Jayden","Matthew", "Julian", "Ryan", "Nathan", "Leo",
				"Caleb", "Hunter", "Christian", "Eli", "Aaron","Ezra", "Adrian", "Mason", "Grayson", "Jeremiah"
			},
			.lastNames = {
				"Smith", "Johnson", "Williams", "Brown", "Jones","Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
				"Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson","Thomas", "Taylor", "Moore", "Jackson", "Martin",
				"Lee", "Perez", "Thompson", "White", "Harris","Sanchez", "Clark", "Ramirez", "Lewis", "Robinson",
				"Walker", "Young", "Allen", "King", "Wright","Scott", "Torres", "Nguyen", "Hill", "Flores",
				"Green", "Adams", "Nelson", "Baker", "Hall","Rivera", "Campbell", "Mitchell", "Carter", "Roberts",
				"Gomez", "Phillips", "Evans", "Turner", "Diaz","Parker", "Cruz", "Edwards", "Collins", "Reyes",
				"Stewart", "Morris", "Morales", "Murphy", "Cook","Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper",
				"Peterson", "Bailey", "Reed", "Kelly", "Howard","Ramos", "Kim", "Cox", "Ward", "Richardson",
				"Watson", "Brooks", "Chavez", "Wood", "James","Bennett", "Gray", "Mendoza", "Ruiz", "Hughes",
				"Price", "Alvarez", "Castillo", "Sanders", "Patel","Myers", "Long", "Ross", "Foster", "Jimenez"
			}
		};

		nameByNationality["Canadian"] = {
			.firstNames = {
				"Noah", "Liam", "Theodore", "Leo", "William","Oliver", "Lucas", "James", "Benjamin", "Henry",
				"Jack", "Levi", "Ethan", "Logan", "Owen","Alexander", "Elijah", "Mateo", "Jackson", "Daniel",
				"Michael", "Mason", "Hudson", "Samuel", "David","Matthew", "Jacob", "Gabriel", "Carter", "Isaac",
				"Thomas", "Caleb", "Ryan", "Nathan", "John","Joseph", "Luke", "Dylan", "Andrew", "Grayson",
				"Wyatt", "Jayden", "Elias", "Miles", "Bennett","Theo", "Emmett", "Beau", "Finn", "Kai"
			},
			.lastNames = {
				"Smith", "Brown", "Tremblay", "Martin", "Roy","Gagnon", "Lee", "Wilson", "Johnson", "MacDonald",
				"Taylor", "Campbell", "Anderson", "Jones", "Leblanc","Cote", "Williams", "White", "Gauthier", "Morin",
				"Bouchard", "Belanger", "Thompson", "Boucher", " Pelletier","Poirier", "Miller", "Beaulieu", "Lefebvre", "Fortin",
				"Gagne", "Bergeron", "Dupuis", "Moreau", "Simard","Graham", "Martel", "Desjardins", "Lavoie", "Bertrand",
				"Savard", "Lachance", "Benoit", "Levesque", "Caron","Grenier", "Ouellet", "Poulin", "Thibault", "Girard",
				"Blais", "Gaudreault", "Turcotte", "Couture", "Lessard","Lemieux", "Hebert", "Perreault", "Fontaine", "Leduc",
				"Dube", "St-Pierre", "Gallant", "Lamontagne", "Rivard","Allard", "Lapointe", "Vaillancourt", "Samson", "Gosselin",
				"Cloutier", "Parent", "Desrosiers", "Lemay", "Houle","Begin", "Trudeau","Tootoo","Crosby","Cook",
				"Taylor","Clark", "Hall", "Baker", "Hill", "Scott","Green", "Adams", "Nelson", "Carter",
				"Mitchell","Roberts", "Phillips", "Evans", "Turner","Parker","Collins", "Stewart", "Morris", "Murphy"
			}
		};

		nameByNationality["Swedish"] = {
			.firstNames = {
				"Noah", "Hugo", "Liam", "Nils", "Alfred", "August", "Oliver", "William","Leo", "Otto",
				"Adam", "Elias", "Theo", "Emil", "Erik", "Oscar", "Arvid","Alexander", "Ludvig", "Filip",
				"Viktor", "Gustav", "Lucas", "Elliot","Axel", "Milo", "Theo", "Samuel", "Benjamin", "Melvin",
				"Leon", "Isak","Gabriel", "Mohamed", "Albin", "Noel", "Theo", "Henry", "Loui", "Adrian",
				"Elton", "Vincent", "Love", "Ivar", "Wilmer", "Elton", "Charlie", "Max","Theo", "Felix"

			},
			.lastNames = {
				"Johansson", "Andersson", "Karlsson", "Nilsson", "Eriksson","Larsson", "Olsson", "Persson", "Svensson", "Gustafsson",
				"Pettersson", "Jonsson", "Jansson", "Hansson", "Bengtsson","Magnusson", "Lindberg", "Lindström", "Lundberg", "Lindgren",
				"Bergström", "Lundgren", "Johansson", "Andersson", "Karlsson","Nilsson", "Eriksson", "Larsson", "Olsson", "Persson",
				"Svensson", "Gustafsson", "Pettersson", "Jonsson", "Jansson","Hansson", "Bengtsson", "Magnusson", "Lindberg", "Lindström",
				"Lundberg", "Lindgren", "Bergström", "Lundgren", "Berg","Jakobsson", "Olofsson", "Danielsson", "Eriksson", "Larsson",
				"Holm", "Lundqvist", "Forsberg", "Söderberg", "Wallin","Ekström", "Hermansson", "Samuelsson", "Isaksson", "Nyström",
				"Berglund", "Sandberg", "Sjöberg", "Åberg", "Blom","Claesson", "Viklund", "Dahl", "Hellström", "Falk",
				"Sundberg", "Ström", "Åström", "Wiklund", "Hedlund","Björklund", "Eliasson", "Lundin", "Fransson", "Öberg",
				"Månsson", "Hedberg", "Blomqvist", "Arvidsson", "Dahlberg","Ek", "Ljung", "Skoglund", "Björk", "Norberg",
				"Löfgren", "Sundström", "Vallin", "Melin", "Ask","Fredriksson", "Håkansson", "Nordin", "Lundholm", "Wikström"
			}
		};

		nameByNationality["Russian"] = {
			.firstNames = {
				"Aleksandr", "Mikhail", "Artyom", "Ivan", "Dmitry", "Maxim", "Matvey", "Lev", "Mark", "Roman",
				"Kirill", "Timofey", "Nikita", "Daniil", "Sergei", "Andrei", "Alexey", "Yegor", "Vladislav", "Ilya",
				"Bogdan", "Makar", "Saveliy", "Demid", "Platon", "Luka", "Miroslav", "Savva", "Leon", "Adam",
				"Boris", "Nikolai", "Evgeny", "Pavel", "Artem", "Stepan", "Grigory", "Fedor", "Konstantin", "Vasiliy",
				"Yaroslav", "Denis", "Georgiy", "Semyon", "Vladimir", "Anton", "Igor", "Oleg", "Ruslan", "Taras"
			},
			.lastNames = {
				"Ivanov", "Smirnov", "Kuznetsov", "Popov", "Petrov","Sokolov", "Lebedev", "Kozlov", "Novikov", "Morozov",
				"Volkov", "Alekseev", "Leontiev", "Semenov", "Egorov","Pavlov", "Kozlov", "Stepanov", "Nikolaev", "Orlov",
				"Andreev", "Makarov", "Nikitin", "Zakharov", "Kuzmin","Gromov", "Fedorov", "Davydov", "Belyaev", "Gusev",
				"Kiselev", "Ilyin", "Sorokin", "Mikhailov", "Tikhonov","Frolov", "Chernov", "Gorbunov", "Mironov", "Lazarev",
				"Medvedev", "Ershov", "Romanov", "Zaitsev", "Belov","Komarov", "Kiselev", "Gulyaev", "Titov", "Kuznetsov",
				"Kudryavtsev", "Bykov", "Zuev", "Tretyakov", "Savinov","Panov", "Ryabov", "Ponomarev", "Golubev", "Kravtsov",
				"Rodionov", "Sobolev", "Zhukov", "Vorobyov", "Naumov","Shcherbakov", "Denisov", "Konstantinov", "Sitnikov", "Kudryashov",
				"Bogdanov", "Solovyov", "Vasilev", "Pavlov", "Anisimov","Polikarpov", "Grigoriev", "Lukin", "Prokhorov", "Nesterov",
				"Kharitonov", "Agafonov", "Efremov", "Filippov", "Krylov","Matveev", "Afanasyev", "Vlasov", "Dorofeev", "Gavrilov",
				"Zotov", "Ignatov", "Kapustin", "Kirillov", "Moiseyev","Osin", "Pakhomov", "Rybakov", "Suvorov", "Tarasov"
			}
		};

		nameByNationality["Finnish"] = {
			.firstNames = {
				"Oliver", "Hannnu", "Jesse", "Leo", "Elias", "Onni", "Tomi", "Henri", "Joonas",
				"Eeli", "Toivo", "Noel", "Leevi", "Daniel", "Otto", "Aatos", "Eemil",
				"Ville", "Mika", "Joel", "Aki", "Matias", "Mikael", "Anton", "Luukas",
				"Timo", "Niilo", "Lenni", "Iivo", "Jere", "Olavi", "Eemeli", "Julius",
				"Urho", "Saku", "Eetu", "Aaron", "Viljami", "Teemu", "Luukas", "Mikko",
				"Rasmus", "Max", "Samuel", "Ilmari", "Matias", "Joona", "Juho", "Petteri","Tapani"
			},
			.lastNames = {
				"Korhonen", "Virtanen", "Mäkinen", "Nieminen", "Mäkelä","Hämäläinen", "Laine", "Heikkinen", "Koskinen", "Järvinen",
				"Lehtinen", "Leinonen", "Laakso", "Hakala", "Koski","Hiltunen", "Räsänen", "Karjalainen", "Seppälä", "Kinnunen",
				"Turunen", "Salo", "Mattila", "Lahtinen", "Ahonen","Vainio", "Tuominen", "Laitinen", "Rantanen", "Kangas",
				"Mikkonen", "Mustonen", "Kivelä", "Pitkänen", "Manninen","Kettunen", "Hintikka", "Toivonen", "Jokinen", "Niskanen",
				"Saarinen", "Pulkkinen", "Partanen", "Hyytiäinen", "Pesonen","Kallio", "Kujala", "Rauhala", "Sorjonen", "Vehviläinen",
				"Paavola", "Pajunen", "Huhtala", "Pirinen", "Savolainen","Alatalo", "Pääkkönen", "Kapanen", "Nousiainen", "Harjula",
				"Kauppinen", "Kemppainen", "Korpela", "Latvala", "Pohjola","Räisänen", "Sillanpää", "Taskinen", "Uusitalo", "Väisänen",
				"Anttila", "Haapanen", "Jokela", "Kauppi", "Koivisto","Kärkkäinen", "Laukkanen", "Myllylä", "Nummela", "Oikarinen",
				"Peltola", "Pietilä", "Pohjonen", "Puurunen", "Rinne","Ruotsalainen", "Saarela", "Salmela", "Särkinen", "Tervo",
				"Tiainen", "Tolonen", "Vaara", "Valkonen", "Vartiainen","Viitala", "Vuorinen", "Ylinen", "Virolainen", "Aaltonen"
			}
		};
		nameByNationality["Czech"] = {
			.firstNames = {
				"Jakub", "Matyáš", "Jan", "Adam", "Matěj", "David", "Vojtěch", "Filip", "Dominik", "Tomáš",
				"Antonín", "Ondřej", "Lukáš", "Martin", "Daniel", "Štěpán", "Tobiáš", "Marek", "Samuel", "Jiří",
				"Václav", "Šimon", "Kryštof", "Jonáš", "Mikuláš", "Oliver", "Tadeáš", "Patrik", "Josef", "František",
				"Jáchym", "Pavel", "Viktor", "Sebastian", "Karel", "Michal", "Petr", "Prokop", "Radim", "Richard",
				"Robin", "Teodor", "Theo", "Vít", "Zdeněk", "Albert", "Aleš", "Bohumil", "Dalibor", "Emanuel"
			},
			.lastNames = {
				"Novák", "Svoboda", "Novotný", "Dvořák", "Černý","Procházka", "Kučera", "Veselý", "Horák", "Němec",
				"Marek", "Pospíšil", "Pokorný", "Hájek", "Král","Jelínek", "Růžička", "Beneš", "Fiala", "Sedláček",
				"Doležal", "Šimek", "Kopecký", "Malý", "Kovář","Kadlec", "Žďárský", "Šťastný", "Ševčík", "Matoušek",
				"Urban", "Říha", "Špaček", "Bláha", "Kříž","Soukup", "Musil", "Šulc", "Moravec", "Krejčí",
				"Hruška", "Kovářík", "Havlíček", "Mach", "Zeman","Kubíček", "Vlček", "Holub", "Štěpánek", "Čech",
				"Bartoš", "Tichý", "Valenta", "Kunc", "Hrubý","Mareš", "Vaněk", "Havel", "Tuček", "Štěpán",
				"Čapek", "Sýkora", "Kudrna", "Vrba", "Matějka","Kolář", "Hrušková", "Dvorský", "Polák", "Švec",
				"Kocián", "Janda", "Kovářová", "Bureš", "Kohout","Mašek", "Špačková", "Žák", "Holeček", "Fischer",
				"Richter", "Schneider", "Wagner", "Kraus", "Müller","Bauer", "Weber", "Hoffmann", "Schmidt", "Klein",
				"Novotná", "Svobodová", "Dvořáková", "Černá", "Procházková","Kučerová", "Veselá", "Horáková", "Němcová"
			}
		};

		nameByNationality["Slovak"] = {
			.firstNames = {
				"Jakub", "Samuel", "Oliver", "Adam", "Michal", "Tomáš", "Filip", "Tobias", "Šimon", "Matias",
				"Matej", "Martin", "Lukáš", "Dominik", "Peter", "Alex", "Richard", "Dávid", "Patrik", "Matúš",
				"Timotej", "Sebastian", "Ján", "Daniel", "Marek", "Vojtech", "Teodor", "Lukáš", "Nikolas", "Gabriel",
				"Juraj", "Milan", "Igor", "Róbert", "Pavol", "Andrej", "Erik", "Kristián", "Branislav", "Boris",
				"Vladimír", "Jozef", "Stanislav", "Karol", "Miroslav", "Roman", "Zoltán", "Ladislav", "Norbert", "Tadeáš"
			},
			.lastNames = {
				"Horváth", "Kováč", "Varga", "Tóth", "Nagy","Szabó", "Molnár", "Baláž", "Balog", "Lukáč",
				"Novák", "Kováčová", "Vargová", "Nagyová", "Tóthová","Horváthová", "Balážová", "Molnárová", "Balogová", "Lukáčová",
				"Kis", "Hudák", "Halász", "Bodnár", "Kiss","Takács", "Farkas", "Kozma", "Gábor", "Jakub",
				"Bartoš", "Gašpar", "Rybár", "Polák", "Sabol","Sedlák", "Kmeť", "Pavlik", "Kollár", "Gregor",
				"Krajčík", "Dubec", "Mészáros", "Szőke", "Vojtek","Adamčík", "Babjak", "Čapkovič", "Danek", "Farkaš",
				"Hruška", "Jurčo", "Kmeťko", "Lacko", "Majer","Nemec", "Ondruš", "Pavol", "Riš", "Ševčík",
				"Tkáč", "Uhrin", "Vasiľ", "Zajac", "Žiga","Boháč", "Čierny", "Dudáš", "Fabian", "Gonda",
				"Hodás", "Ivan", "Jano", "Kolesár", "Lukáš","Matúš", "Novomeský", "Oravec", "Pavúk", "Rybnikár",
				"Slanina", "Šimko", "Tatar", "Urbánik", "Vašek","Zeman", "Andrus", "Bílek", "Černý", "Dohnal",
			}
		};
	}
		NameGenerator() {
			loadNames();
		}
		FullName getRandomName(const std::string& nationality) {
			std::mt19937 rng(std::random_device{}());
			auto& n = nameByNationality.at(nationality);

			return {
				n.firstNames[std::uniform_int_distribution<int>(0,n.firstNames.size()-1)(rng)],
				n.lastNames[std::uniform_int_distribution<int>(0,n.lastNames.size()-1)(rng)]
			};
		}

		static NameGenerator& GetInstance() {
			static NameGenerator instance{};
			return instance;
		}
};

