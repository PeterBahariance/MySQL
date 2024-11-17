#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

#include "../../includes/sql/sql.h"

using namespace std;






bool test_sql(bool debug = false)
{



const vectorstr command_list = {

/*00*/  "make table NBA fields fname, lname, age, position, team",

/*01*/  "insert into NBA values LeBron, James, 36, Forward, \"Los Angeles Lakers\"",

/*02*/  "insert into NBA values Stephen, Curry, 33, Guard, \"Golden State Warriors\"",

/*03*/  "insert into NBA values Kevin, Durant, 32, Forward, \"Brooklyn Nets\"",

/*04*/  "insert into NBA values Giannis, Antetokounmpo, 26, Forward, \"Milwaukee Bucks\"",

/*05*/  "insert into NBA values James, Harden, 31, Guard, \"Brooklyn Nets\"",

/*06*/  "insert into NBA values Anthony, Davis, 28, Forward, \"Los Angeles Lakers\"",

/*07*/  "insert into NBA values Kawhi, Leonard, 30, Forward, \"Los Angeles Clippers\"",

/*08*/  "insert into NBA values Luka, Doncic, 22, Guard, \"Dallas Mavericks\"",

/*09*/  "insert into NBA values Nikola, Jokic, 26, Center, \"Denver Nuggets\"",

/*10*/  "insert into NBA values Joel, Embiid, 27, Center, \"Philadelphia 76ers\"",

/*11*/  "insert into NBA values Damian, Lillard, 30, Guard, \"Portland Trail Blazers\"",

/*12*/  "insert into NBA values Jayson, Tatum, 23, Forward, \"Boston Celtics\"",

/*13*/  "insert into NBA values Devin, Booker, 24, Guard, \"Phoenix Suns\"",

/*14*/  "insert into NBA values Chris, Paul, 36, Guard, \"Phoenix Suns\"",

/*15*/  "insert into NBA values Zion, Williamson, 21, Forward, \"New Orleans Pelicans\"",

/*16*/  "insert into NBA values Trae, Young, 22, Guard, \"Atlanta Hawks\"",

/*17*/  "insert into NBA values Jimmy, Butler, 31, Forward, \"Miami Heat\"",

/*18*/  "insert into NBA values Bradley, Beal, 28, Guard, \"Washington Wizards\"",

/*19*/  "insert into NBA values \"Karl-Anthony\", Towns, 25, Center, \"Minnesota Timberwolves\"",




//   /*00*/  "make table students fields  fname, lname, gender, age, major, GradYear",
//   /*01*/  "insert into students values Peter, Bahariance, Male, 19, CS, 2026", 

//   /*02*/  "insert into students values Lizzy, Esguerra, Female, 20, MathCS, 2026",
//   /*03*/  "insert into students values Michael, Mansour, Male, 23, Physics, 2021",
//   /*04*/  "insert into students values Danica, Abnoos, Female, 17, Chemistry, 2028",
//   /*05*/  "insert into students values Kiet, Dang, Male, 25, Biology, 2022",
//   /*06*/  "insert into students values Daphne, Nea, Female, 19, English, 2023",
//   /*07*/  "insert into students values Kyle, Lee, Male, 22, History, 2021",
//   /*08*/  "insert into students values Candace, Parker, Female, 30, SportsEd, 2024",
//   /*09*/  "insert into students values Derrick, Abnoos, Male, 21, English, 2026",
//   /*10*/  "insert into students values Sharon, Cooper, Female, 23, Music, 2023",
//   /*10*/  "insert into students values Michael, Amour, Male, 27, Art, 2023",

//  /*11*/  "select * from students",

//   /*12*/  "select * from students where gender = Male or lname = Abnoos",
//   /*13*/  "select * from students where age > 20 and major = CS",
//   /*14*/  "select * from students where age < 18 or age > 25 and gender = Female",
//   /*15*/  "select * from students where major = MathCS and GradYear = 2023 or fname = John",
//   /*16*/  "select * from students where lname = Smith or age > 30 and major = Biology",
//   /*17*/  "select * from students where age < 25 and major = English or gender = Female",
//   /*18*/  "select * from students where fname = Michael or lname = Doe and GradYear = 2024",
//   /*19*/  "select * from students where age > 18 and gender = Male or major = Physics",
//   /*20*/  "select * from students where lname = Nguyen or age > 22 and gender = Female",
//   /*21*/  "select * from students where major = Chemistry and GradYear = 2025 or fname = Sarah",

  // /*12*/  "select * from students where (GradYear = 2026) and gender = Male or lname = Abnoos",
  // /*13*/  "select * from students where major = MathCS or ((gender = Male or age > 20 and major = CS) or fname = Michael or GradYear = 2021)",
  // /*14*/  "select * from students where (((major = English)))",
  // /*15*/  "select * from students where ((((((()()(((((fname =  Kyle or age >= 23) and fname = Kiet))))))))))",
  // /*16*/  "select * from students where age > 40",
  // /*17*/  "select * from students where (fname = Anthony or age > 50) and (age < 24 or lname > Cooper) or (fname = Peter and lname < Esguerra)",
  // /*18*/  "select * from students where (fname = Anthony or age > 50) and (age < 24 or lname > Cooper) or (fname = Peter and lname < Abnoos)",

  // /*19*/ "select * from students where (((GradYear = 2026) and (gender = Male)) or ((lname = Abnoos) and (age < 25))) and ((major = CS) or (major = MathCS))",
  // /*20*/ "select * from students where ((major = Biology) and (GradYear > 2022)) or (((fname = Danica) and (age <= 17)) and (lname = Parker)) or (major = Chemistry)",
  // /*21*/ "select * from students where (((fname = Peter) and (lname = Bahariance)) or ((age > 18) and (major = CS))) and ((GradYear = 2026) and (gender = Male))",
  // /*22*/ "select * from students where (((age > 20) or (major = History)) and (lname = Lee)) or (((fname = Kiet) and (age >= 25)) and (GradYear = 2022))",
  
// /*23*/ "select * from students where (GradYear = 2026 and (gender = Male or major = CS))",

// /*24*/ "select * from students where ((fname = Danica or fname = Peter) and (age < 18 or GradYear = 2028))",

// /*25*/ "select * from students where (major = MathCS and (age > 20 or (gender = Female and GradYear = 2026)))",

// /*26*/ "select * from students where ((age > 23 or lname = Lee) and (major = History or GradYear = 2021))",

// /*27*/ "select * from students where ((fname = Sharon and (major = Music or age > 22)) and GradYear = 2023)",

// /*28*/ "select * from students where (lname = Abnoos and (fname = Derrick or fname = Danica))",

// /*29*/ "select * from students where (major = English and (GradYear < 2024 or (age = 19 and gender = Female)))",

// /*30*/ "select * from students where (fname = Michael and (lname = Amour or lname = Mansour))",

// /*31*/ "select * from students where (GradYear = 2026 and (gender = Female or lname = Abnoos))",

// /*32*/ "select * from students where (age > 20 and (major = CS or (GradYear = 2023 and gender = Male)))",
  

// /*32*/ "select * from students where ( (age > 20 or major = CS ) and ( GradYear = 2021 or fname = Derrick ) )"
  
  
};

  SQL sql;


  for(int i = 0; i < command_list.size(); i++)
  {
    //string inputCommand;
    // cout << "Enter you command: ";
    // getline(cin, inputCommand);

    sql.command(command_list[i]);
    //cout << "Here are the FINAL RECNOS: " << sql.select_recnos() << endl;

  }


  for(int i = 0; i < 50; i++)
  {
    string inputCommand;
    cout << "Enter you command: ";
    getline(cin, inputCommand);

    cout << sql.command(inputCommand) << endl;

    if(!sql.emptyRec())
      cout << "Here are the recnos: " << sql.select_recnos() << endl;

    else  
      cout << "No corresponding records!" << endl;

  }




  
  return true;
}

TEST(TEST_SQL, TestSQL) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_sql(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}



/*







*/