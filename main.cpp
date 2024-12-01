#include "main.h"

int main()
{


	LI_FN(Sleep)(400);
	LI_FN(printf)(("\n \033[0m[\033[1;32m+\033[0m]"));

	(slowPrint)((" License Is Valid"), std::chrono::milliseconds(60));

	LI_FN(Sleep)(400);

	LI_FN(system)(("cls"));


main:




	LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m1\033[0m]"));

	(slowPrint)((" Load Bypass"), std::chrono::milliseconds(60));


	LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m2\033[0m]"));

	(slowPrint)((" Load Cheat"), std::chrono::milliseconds(60));


	LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m!\033[0m]"));
	(slowPrint)((" Options: "), std::chrono::milliseconds(60));

	
	std::cin >> choice;

	if (choice == 1)
	{
	
	}




	


	if (choice == 2)
	{

	

		hide_LI_FNs();

		auto status = Bypass::SetupDriver();

		if (status)
		{
			LI_FN(Sleep)(400);
			LI_FN(printf)(("\n \033[0m[\033[1;32m+\033[0m]"));

			(slowPrint)((" Emulation Is Working"), std::chrono::milliseconds(60));

			LI_FN(Sleep)(400);

			LI_FN(system)(("cls"));

		}
		else
		{
			LI_FN(Sleep)(400);
			LI_FN(printf)(("\n \033[0m[\033[1;31m-\033[0m]"));

			(slowPrint)((" Emulation Failed"), std::chrono::milliseconds(60));

			LI_FN(Sleep)(400);

			LI_FN(system)(("cls"));
			LI_FN(exit)(0);
		}

		LI_FN(Sleep)(400);
		LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m!\033[0m]"));

		(slowPrint)((" Waiting For Game"), std::chrono::milliseconds(60));

		LI_FN(Sleep)(400);


		while (windowid == NULL)

		{
			XorS(wind, ("Fortnite  "));
			windowid = FindWindowA_Spoofed(0, wind.decrypt());

		}

		Fort_windwo = FindWindowA_Spoofed(NULL, ("Fortnite  "));
		if (Fort_windwo)
		{
			//JunkCode();
			// ThreadCheck();
			LI_FN(system)(("cls"));
			LI_FN(Sleep)(400);
			LI_FN(printf)(("\n \033[0m[\033[1;32m+\033[0m]"));
			(slowPrint)((" Found Fortnite"), std::chrono::milliseconds(60));
			LI_FN(Sleep)(400);
			LI_FN(system)(("cls"));
			//KeyAuthApp.check();
			LI_FN(Sleep)(400);
			LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m!\033[0m]"));
			(slowPrint)((" Getting PID"), std::chrono::milliseconds(60));
			LI_FN(Sleep)(400);
			LI_FN(system)(("cls"));

			Bypass::ProcID = Bypass::Attach(("FortniteClient-Win64-Shipping.exe"));

			if (Bypass::ProcID)
			{
				LI_FN(Sleep)(400);
				LI_FN(printf)(("\n \033[0m[\033[1;32m+\033[0m]"));
				std::cout << ((" Found PID -> ")) << Bypass::ProcID << std::flush;
				LI_FN(Sleep)(400);
				LI_FN(system)(("cls"));
			}
			else
			{
				LI_FN(Sleep)(400);
				LI_FN(printf)(("\n \033[0m[\033[1;31m-\033[0m]"));
				std::cout << ((" Failed To Find PID")) << std::flush;
				LI_FN(Sleep)(400);
				LI_FN(system)(("cls"));
			}

		
			Bypass::GameBase = Bypass::get_base_address();

			LI_FN(Sleep)(400);
			LI_FN(printf)(("\n \033[0m[\033[0m\033[1;34m!\033[0m]"));

			(slowPrint)((" Getting Game Base"), std::chrono::milliseconds(60));

			LI_FN(Sleep)(400);

			LI_FN(system)(("cls"));

			if (Bypass::GameBase)
			{
				LI_FN(Sleep)(400);
				LI_FN(printf)(("\n \033[0m[\033[1;32m+\033[0m]"));
				std::cout << ((" Found Game Base -> ")) << Bypass::GameBase << std::flush;
				LI_FN(Sleep)(400);
				LI_FN(system)(("cls"));
			}
			else
			{
				LI_FN(Sleep)(400);
				LI_FN(printf)(("\n \033[0m[\033[1;31m-\033[0m]"));
				std::cout << ((" Failed To Game Base")) << std::flush;
				LI_FN(Sleep)(400);
				LI_FN(system)(("cls"));
			}
			Bypass::RestartBuffers();

			Bypass::BypassControlRegister3();

			hook_mouse->HOOK();

			thread([&]() { cache_players(); }).detach();



			
			


			floot->levels_call();



			//thread([&]() { PlayerFlyThread(); }).detach();


		//	KeyAuthApp.check();
		//	ShowWindow_Spoofed(GetConsoleWindow(), SW_HIDE);
			Beep(300, 1000);


			hijack::hijack_init();
			directx_init();
			render_loop();
		}
	}

}