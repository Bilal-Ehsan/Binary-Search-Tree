#include "bst.h"

// NOTE: Required before the include below
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE bst_tests

#include <boost/test/unit_test.hpp>

using Dict = BST;
using keyType = Dict::keyType;
using itemType = Dict::itemType;

// Utility functions

void isPresent(Dict& dict, keyType k, itemType i) {
  itemType* p_i = dict.lookup(k);

  BOOST_CHECK_MESSAGE(p_i, std::to_string(k) + " is missing");

  if (p_i) {
    BOOST_CHECK_MESSAGE(*p_i == i,
      std::to_string(k) + " should be " + i + ", but found " + *p_i);
  }
}

void isAbsent(Dict& dict, keyType k) {
  BOOST_CHECK_MESSAGE(dict.lookup(k) == nullptr,
    std::to_string(k) + " should be absent, but is present");
}

void insertTestData(Dict& dict) {
  dict.insert(9, "Edward");
  dict.insert(22, "Jane");
  dict.insert(22, "Mary");
  dict.insert(0, "Harold");
  dict.insert(37, "Victoria");
  dict.insert(4, "Matilda");
  dict.insert(26, "Oliver");
  dict.insert(42, "Elizabeth");
  dict.insert(19, "Henry");
  dict.insert(4, "Stephen");
  dict.insert(24, "James");
  dict.insert(-1, "Edward");
  dict.insert(31, "Anne");
  dict.insert(23, "Elizabeth");
  dict.insert(1, "William");
  dict.insert(26, "Charles");
}

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( constructor_tests )

BOOST_AUTO_TEST_CASE( construct_empty ) {
  // Use of "new" is to avoid triggering the destructor in this test
  new Dict();
}

BOOST_AUTO_TEST_CASE( destroy_empty ) { Dict dict; }

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( lookup_insert_tests )

BOOST_AUTO_TEST_CASE( empty_lookup ) {
  Dict dict;
  isAbsent(dict, 1);
}

BOOST_AUTO_TEST_CASE( single_insert ) {
  Dict dict;
  dict.insert(22, "Mary");
}

BOOST_AUTO_TEST_CASE( single_insert_lookup ) {
  Dict dict;

  dict.insert(22, "Mary");
  isPresent(dict, 22, "Mary");
}

BOOST_AUTO_TEST_CASE( single_overwrite_lookup ) {
  Dict dict;

  dict.insert(22, "Jane");
  dict.insert(22, "Mary");

  isPresent(dict, 22, "Mary");
}

BOOST_AUTO_TEST_CASE( multiple_insert ) {
  Dict dict;

  insertTestData(dict);
}

BOOST_AUTO_TEST_CASE( multiple_insert_lookup_present ) {
  Dict dict;
  insertTestData(dict);

  isPresent(dict, 22, "Mary");
  isPresent(dict, 4, "Stephen");
  isPresent(dict, 9, "Edward");
  isPresent(dict, 1, "William");
  isPresent(dict, 0, "Harold");
  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isPresent(dict, 19, "Henry");
  isPresent(dict, 31, "Anne");
  isPresent(dict, 23, "Elizabeth");
  isPresent(dict, 37, "Victoria");
  isPresent(dict, 42, "Elizabeth");
  isPresent(dict, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( multiple_insert_lookup_absent ) {
  Dict dict;
  insertTestData(dict);

  isAbsent(dict, 2);
  isAbsent(dict, 3);
  isAbsent(dict, -4);
  isAbsent(dict, 56);
  isAbsent(dict, 30);
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( remove_tests )

BOOST_AUTO_TEST_CASE( empty_remove ) {
  Dict dict;

  dict.remove(43);
  isAbsent(dict, 43);
}

BOOST_AUTO_TEST_CASE( remove_childless_root ) {
  Dict dict;

  dict.insert(7, "John");
  dict.remove(7);
   
  isAbsent(dict, 7);
}

BOOST_AUTO_TEST_CASE( remove_left_child_of_root ) {
  Dict dict;

  dict.insert(31, "Anne");
  dict.insert(7, "John");

  dict.remove(7);

  isAbsent(dict, 7);
  isPresent(dict, 31, "Anne");
}

BOOST_AUTO_TEST_CASE( remove_right_child_of_root ) {
  Dict dict;

  dict.insert(7, "John");
  dict.insert(31, "Anne");
  dict.remove(31);

  isAbsent(dict, 31);
  isPresent(dict, 7, "John");
}

BOOST_AUTO_TEST_CASE( insert_many_remove_childless_nodes ) {
  Dict dict;

  insertTestData(dict);

  dict.remove(-1);
  isAbsent(dict,-1);

  dict.remove(1);
  isAbsent(dict, 1);

  dict.remove(19);
  isAbsent(dict, 19);

  dict.remove(23);
  isAbsent(dict, 23);

  dict.remove(31);
  isAbsent(dict, 31);

  dict.remove(42);
  isAbsent(dict, 42);

  dict.remove(4);
  isAbsent(dict, 4);

  isPresent(dict, 22, "Mary");
  isPresent(dict, 9, "Edward");
  isPresent(dict, 0, "Harold");
  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isPresent(dict, 37, "Victoria");
}

BOOST_AUTO_TEST_CASE( overwrite_then_remove ) {
  Dict dict;

  dict.insert(22, "Jane");
  dict.insert(22, "Mary");
  dict.insert(4, "Matilda");
  dict.insert(26, "Oliver");
  dict.insert(4, "Stephen");
  dict.insert(26, "Charles");

  dict.remove(4);
  isAbsent(dict, 4);

  dict.remove(26);
  isAbsent(dict, 26);

  dict.remove(22);
  isAbsent(dict, 22);
}

BOOST_AUTO_TEST_CASE( remove_root_with_left_child ) {
  Dict dict;

  dict.insert(31, "Anne");
  dict.insert(7, "John");

  dict.remove(31);

  isAbsent(dict, 31);
  isPresent(dict, 7, "John");
}

BOOST_AUTO_TEST_CASE( remove_root_with_right_child ) {
  Dict dict;

  dict.insert(7, "John");
  dict.insert(31, "Anne");

  dict.remove(31);

  isAbsent(dict, 31);
  isPresent(dict, 7, "John");
}

BOOST_AUTO_TEST_CASE( insert_many_remove_nodes_with_one_child ) {
  Dict dict;

  insertTestData(dict);

  dict.remove(4);
  isAbsent(dict, 4);

  dict.remove(1);
  isAbsent(dict, 1);

  dict.remove(9);
  isAbsent(dict, 9);

  isPresent(dict, 22, "Mary");
  isPresent(dict, 0, "Harold");
  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isPresent(dict, 19, "Henry");
  isPresent(dict, 31, "Anne");
  isPresent(dict, 23, "Elizabeth");
  isPresent(dict, 37, "Victoria");
  isPresent(dict, 42, "Elizabeth");
  isPresent(dict, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( remove_root_with_children ) {
  Dict dict;

  dict.insert(31, "Anne");
  dict.insert(7, "John");
  dict.insert(42, "Elizabeth");
  dict.remove(31);

  isAbsent(dict, 31);
  isPresent(dict, 7, "John");
  isPresent(dict, 42, "Elizabeth");
}

BOOST_AUTO_TEST_CASE( insert_many_remove_nodes_with_children ) {
  Dict dict;

  insertTestData(dict);

  dict.remove(0);
  isAbsent(dict, 0);

  dict.remove(37);
  isAbsent(dict, 37);

  dict.remove(22);
  isAbsent(dict, 22);

  isPresent(dict, 4, "Stephen");
  isPresent(dict, 9, "Edward");
  isPresent(dict, 1, "William");
  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isPresent(dict, 19, "Henry");
  isPresent(dict, 31, "Anne");
  isPresent(dict, 23, "Elizabeth");
  isPresent(dict, 42, "Elizabeth");
  isPresent(dict, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( insert_many_remove_absent ) {
  Dict dict;

  insertTestData(dict);

  dict.remove(6);
  isAbsent(dict, 6);

  isPresent(dict, 22, "Mary");
  isPresent(dict, 4, "Stephen");
  isPresent(dict, 9, "Edward");
  isPresent(dict, 1, "William");
  isPresent(dict, 0, "Harold");
  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isPresent(dict, 19, "Henry");
  isPresent(dict, 31, "Anne");
  isPresent(dict, 23, "Elizabeth");
  isPresent(dict, 37, "Victoria");
  isPresent(dict, 42, "Elizabeth");
  isPresent(dict, -1, "Edward");
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( copy_constructor_tests )

BOOST_AUTO_TEST_CASE( copy_constructor_fully_copies ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2(dict_1);  

  isPresent(dict_2, 22, "Mary");
  isPresent(dict_2, 4, "Stephen");
  isPresent(dict_2, 9, "Edward");
  isPresent(dict_2, 1, "William");
  isPresent(dict_2, 0, "Harold");
  isPresent(dict_2, 24, "James");
  isPresent(dict_2, 26, "Charles");
  isPresent(dict_2, 19, "Henry");
  isPresent(dict_2, 31, "Anne");
  isPresent(dict_2, 23, "Elizabeth");
  isPresent(dict_2, 37, "Victoria");
  isPresent(dict_2, 42, "Elizabeth");
  isPresent(dict_2, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( copy_constructor_does_not_delete_source ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2(dict_1);

  isPresent(dict_1, 22, "Mary");
  isPresent(dict_1, 4, "Stephen");
  isPresent(dict_1, 9, "Edward");
  isPresent(dict_1, 1, "William");
  isPresent(dict_1, 0, "Harold");
  isPresent(dict_1, 24, "James");
  isPresent(dict_1, 26, "Charles");
  isPresent(dict_1, 19, "Henry");
  isPresent(dict_1, 31, "Anne");
  isPresent(dict_1, 23, "Elizabeth");
  isPresent(dict_1, 37, "Victoria");
  isPresent(dict_1, 42, "Elizabeth");
  isPresent(dict_1, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( copy_constructor_is_deep ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2(dict_1);

  dict_1.insert(2, "William");
  isPresent(dict_1, 2, "William");
  isAbsent(dict_2, 2);

  dict_2.insert(3, "Henry");
  isPresent(dict_2, 3, "Henry");
  isAbsent(dict_1, 3);

  dict_1.remove(24);
  isAbsent(dict_1, 24);
  isPresent(dict_2, 24, "James");

  dict_2.remove(26);
  isAbsent(dict_2, 26);
  isPresent(dict_1, 26, "Charles");
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( copy_assignment_tests )

BOOST_AUTO_TEST_CASE( copy_assignment_fully_copies ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2 = dict_1;

  isPresent(dict_2, 22, "Mary");
  isPresent(dict_2, 4, "Stephen");
  isPresent(dict_2, 9, "Edward");
  isPresent(dict_2, 1, "William");
  isPresent(dict_2, 0, "Harold");
  isPresent(dict_2, 24, "James");
  isPresent(dict_2, 26, "Charles");
  isPresent(dict_2, 19, "Henry");
  isPresent(dict_2, 31, "Anne");
  isPresent(dict_2, 23, "Elizabeth");
  isPresent(dict_2, 37, "Victoria");
  isPresent(dict_2, 42, "Elizabeth");
  isPresent(dict_2, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( copy_assignment_overwrites ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2.insert(22, "Jane");
  dict_2.insert(2, "William");

  dict_1 = dict_2;

  isAbsent(dict_1, 4);
  isAbsent(dict_1, 9);
  isAbsent(dict_1, 1);
  isAbsent(dict_1, 0);
  isAbsent(dict_1, 24);
  isAbsent(dict_1, 26);
  isAbsent(dict_1, 19);
  isAbsent(dict_1, 31);
  isAbsent(dict_1, 23);
  isAbsent(dict_1, 37);
  isAbsent(dict_1, 42);
  isAbsent(dict_1, -1);

  isPresent(dict_1, 2, "William");
  isPresent(dict_1, 22, "Jane");
}

BOOST_AUTO_TEST_CASE( copy_assignment_does_not_reverse_copy ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;

  dict_2.insert(22, "Jane");
  dict_2.insert(2, "William");

  dict_2 = dict_1;

  isAbsent(dict_1, 2);
}

BOOST_AUTO_TEST_CASE( copy_assignment_is_deep ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2 = dict_1;

  dict_1.insert(2, "William");
  isPresent(dict_1, 2, "William");
  isAbsent(dict_2, 2);

  dict_2.insert(3, "Henry");
  isPresent(dict_2, 3, "Henry");
  isAbsent(dict_1, 3);

  dict_1.remove(24);
  isAbsent(dict_1, 24);
  isPresent(dict_2, 24, "James");

  dict_2.remove(26);
  isAbsent(dict_2, 26);
  isPresent(dict_1, 26, "Charles");
}

BOOST_AUTO_TEST_CASE( copy_self_assignment ) {
  Dict dict;
  insertTestData(dict);

  dict = dict;

  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
  isAbsent(dict, 2);
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( move_constructor_tests )

BOOST_AUTO_TEST_CASE( move_constructor_fully_moves ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2(std::move(dict_1));

  isPresent(dict_2, 22, "Mary");
  isPresent(dict_2, 4, "Stephen");
  isPresent(dict_2, 9, "Edward");
  isPresent(dict_2, 1, "William");
  isPresent(dict_2, 0, "Harold");
  isPresent(dict_2, 24, "James");
  isPresent(dict_2, 26, "Charles");
  isPresent(dict_2, 19, "Henry");
  isPresent(dict_2, 31, "Anne");
  isPresent(dict_2, 23, "Elizabeth");
  isPresent(dict_2, 37, "Victoria");
  isPresent(dict_2, 42, "Elizabeth");
  isPresent(dict_2, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( move_constructor_steals ) {
  Dict* dictPtr;

  {
    Dict dict_1;
    insertTestData(dict_1);

    dictPtr = new Dict(std::move(dict_1));

    isAbsent(dict_1,22);
    isAbsent(dict_1, 4);
    isAbsent(dict_1, 9);
    isAbsent(dict_1, 1);
    isAbsent(dict_1, 0);
    isAbsent(dict_1, 24);
    isAbsent(dict_1, 26);
    isAbsent(dict_1, 19);
    isAbsent(dict_1, 31);
    isAbsent(dict_1, 23);
    isAbsent(dict_1, 37);
    isAbsent(dict_1, 42);
    isAbsent(dict_1, -1);

    // dict_1 gets deleted here
  }

  isPresent(*dictPtr, 22, "Mary");
  isPresent(*dictPtr, 4, "Stephen");
  isPresent(*dictPtr, 9, "Edward");
  isPresent(*dictPtr, 1, "William");
  isPresent(*dictPtr, 0, "Harold");
  isPresent(*dictPtr, 24, "James");
  isPresent(*dictPtr, 26, "Charles");
  isPresent(*dictPtr, 19, "Henry");
  isPresent(*dictPtr, 31, "Anne");
  isPresent(*dictPtr, 23, "Elizabeth");
  isPresent(*dictPtr, 37, "Victoria");
  isPresent(*dictPtr, 42, "Elizabeth");
  isPresent(*dictPtr, -1, "Edward");

  delete dictPtr;
}

BOOST_AUTO_TEST_SUITE_END()

//////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( move_assignment_tests )

BOOST_AUTO_TEST_CASE( move_assignment_fully_moves ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2 = std::move(dict_1);

  isPresent(dict_2, 22, "Mary");
  isPresent(dict_2, 4, "Stephen");
  isPresent(dict_2, 9, "Edward");
  isPresent(dict_2, 1, "William");
  isPresent(dict_2, 0, "Harold");
  isPresent(dict_2, 24, "James");
  isPresent(dict_2, 26, "Charles");
  isPresent(dict_2, 19, "Henry");
  isPresent(dict_2, 31, "Anne");
  isPresent(dict_2, 23, "Elizabeth");
  isPresent(dict_2, 37, "Victoria");
  isPresent(dict_2, 42, "Elizabeth");
  isPresent(dict_2, -1, "Edward");
}

BOOST_AUTO_TEST_CASE( move_assignment_overwrites ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2.insert(22, "Jane");
  dict_2.insert(2, "William");

  dict_1 = std::move(dict_2);

  isAbsent(dict_1, 4);
  isAbsent(dict_1, 9);
  isAbsent(dict_1, 1);
  isAbsent(dict_1, 0);
  isAbsent(dict_1, 24);
  isAbsent(dict_1, 26);
  isAbsent(dict_1, 19);
  isAbsent(dict_1, 31);
  isAbsent(dict_1, 23);
  isAbsent(dict_1, 37);
  isAbsent(dict_1, 42);
  isAbsent(dict_1, -1);

  isPresent(dict_1, 2, "William");
}

BOOST_AUTO_TEST_CASE( move_assignment_is_not_shallow_copy ) {
  Dict dict_1;
  insertTestData(dict_1);

  Dict dict_2;
  dict_2 = std::move(dict_1);

  dict_1.remove(19);
  dict_1.remove(23);

  isPresent(dict_2, 19, "Henry");
  isPresent(dict_2, 23, "Elizabeth");
}

BOOST_AUTO_TEST_CASE( move_self_assignment ) {
  Dict dict;
  insertTestData(dict);

  dict = std::move(dict);

  isPresent(dict, 24, "James");
  isPresent(dict, 26, "Charles");
}

BOOST_AUTO_TEST_SUITE_END()
