#include <string.h>
#include <stdio.h>
#include <stdlib.h>		// For built-in malloc that returns void *

#include "Dictionary.h"
#include "Enrollment.h"

// create global dictionaries
dictionary	CourseDict;
dictionary	StudentDict;

// -------------------------------------------
// TODO: implement addCourse
// adds e to the CourseDict.
// Checks if e already exists, if so print error message
// and don't update the dictionary.  Else update dictionary.
// -------------------------------------------

void addCourse( course_element *e ) {
	
	node_t *course = find(&CourseDict, e -> key);

	if (course != NULL) {
		fprintf(stderr, "Course already exists.");
		return;
	}

	insert(&CourseDict, (element*)e);
}


// -------------------------------------------
// TODO: implement delCourse
// deletes course whose ID is c from the CourseDict.
// This should check if the course to be deleted exists.
// If not, just print an error message: "Course does not exist."
// and do not update both dictionaries.
// If the course exists, then this will also drop students who
// are currently enrolled // in the course i.e. StudentDict needs
// to be updated.  CourseDict needs to be updated as well.
// -------------------------------------------

void delCourse( char *c_key ) {

	node_t *course = find(&CourseDict, c_key);
	if (course == NULL) {
		fprintf(stderr, "\nCourse does not exist");
		return;
	}

	node_t *ptr = ((course_element*)course) -> value -> list;
	while (ptr != NULL) {
		node_t *temp = ptr -> next;
		free(ptr);
		ptr = temp;
	}

	delete(&CourseDict, c_key);

}


// -------------------------------------------
// TODO: implement enroll
// enrolls student into course. params are s_key and c_key
// Need to check for: existence of course, and if student
// is already enrolled in the course.  Also check if course
// is full or if student has reached max number of courses.
// If there's any error, print a message and don't enroll
// the student.  If there's no errors, update both dictionaries.
// -------------------------------------------

void enroll( char *s_key, char *c_key) {


	// declare vars for nodes coressponding to course entry and student entry
	// in respective dictionaries
	node_t *cp = find(&CourseDict, c_key);
	node_t *sp = find(&StudentDict, s_key);


	// decalare vars for course info and student info corresponding to
	// the value fields of each node/element
	// initialize student info to NULL

	// declare course and student node corresponding to list of courses
	// in a student's node and students in a course's node	
	//node_t *course_s_list = (cp -> list);
	//node_t *student_c_list = (sv -> list);


	
	// fetch course and student corresponding to keys from respective dicts
	// initialize vars depending on whether student/course with given key
	// is in the dictionary
	
	// if course doens't exist, print error and return
	if (cp == NULL){
		fprintf(stderr, "\nCourse does not exist.");
		return;
	}

	// if student does not exist, insert student to student dict

	if (sp == NULL) {
		
		// instantiate a 'student_element' pointer struct and set the fields
		// HINT: you can copy the s_key to the key field of student_element
		// using memcpy
		// HINT: you will use the 'newNode' function to instantiate a struct for
		// the value field of the element. See newNode implementation in Enrollment.h
		// the 'value' field of student_element is itself a struct with two fields
		// initialize those fields accordingly
		student_element *se = (student_element*)malloc(sizeof(student_element));
		memcpy(se -> key, s_key, 5);
		se -> value = (student_value*) newNode( 's' );
		se -> value -> num_courses = 0;
		se -> value -> list = NULL;


		// insert student element in the corresponding dictionary
		// HINT: you will need to cast the student element as (element*). See Dictionary.h
		// HINT: after you insert student element into student dict, assign the value of that
		// (newly inserted node) to the student info that you initialized to NULL in the beginning
		printf("Adding student: %s\n", s_key);
		insert(&StudentDict, (element*)se);
	}

		
	// check if student is enrolled in the course. If so, print message and return
	// HINT: navigate to the (student) list of the course
	node_t *check = findList(((course_element*)cp) -> value -> list, s_key);
	if (check != NULL) {
		fprintf(stderr, "\nStudent is already enrolled.");
		return;
	}


	// see if course is full
	if ((((course_element*)cp) -> value -> capacity) == (((course_element*)cp) -> value -> num_students)) {
		fprintf(stderr, "\nCourse is full.");
		return;
	}


	
	// if not, add student to the list (of students) in the course
	// HINT: instantiate a new node and assign it to the node corresp.
	// to student in course's list - initialize the node's fields and
	// insert it into the course's list of students enrolled in it
	// and update the course value fields
	node_t *addStud = (node_t*)malloc(sizeof(node_t));
	memcpy((*addStud).key, s_key, 5);
	(*addStud).next = NULL;
	(*addStud).prev = NULL;
	insertList(&(((course_element*)cp) -> value -> list), addStud);



	// if student already enrolled in '2' courses, print message and return
	node_t *fetch = find(&StudentDict, s_key);
	if ((((student_element*)fetch) -> value -> num_courses) == 2) {
		fprintf(stderr, "\nStudent is already enrolled in 2 classes.");
		return;
	}

	// otherwise, add node to the list of student's enrolled courses
	// HINT: again, instantiate a node (this will be the node of a course
	// in a student's list) and intialize the fields of that node
	node_t *addCourse = (node_t*)malloc(sizeof(node_t));
	memcpy((*addCourse).key, c_key, 5);
	(*addCourse).next = NULL;
	(*addCourse).prev = NULL;
	insertList(&(((student_element*)fetch) -> value -> list), addCourse);

}



// -------------------------------------------
// TODO: implement drop. 
// drops a student from course. params are s_key and c_key
// Need to check for: existence of course, and if student
// is enrolled in the course.  If any error is encountered,
// print an error message.  Do not update either dictionaries.
// If there are no errors, update both dictionaries.
// -------------------------------------------

void drop ( char *s_key, char *c_key ) {

	// fetch nodes corresponding to student and course from
	// respective dicts having the given keys
	node_t *course = find(&CourseDict, c_key);
	node_t *student = find(&StudentDict, s_key);



	// if found no such student or course, print message and return
	if (course == NULL || student == NULL) {
		fprintf(stderr, "\nCourse does not exist.");
		return;
	}



	// see if student is enrolled in the course
	node_t *course_slist = ((course_element*)course) -> value -> list;
	node_t *toDel = findList(course_slist, s_key);

	node_t *student_clist = ((student_element*)student) -> value -> list;
	node_t *toDel2 = findList(student_clist, c_key);

	// delete course from student list
	deleteList(&course_slist, toDel);

	// delete student from course list
	deleteList(&student_clist, toDel2);


	// update counter fields
	((course_element*)course) -> value -> num_students = (((course_element*)course) -> value -> num_students) - 1;
	((student_element*)student) -> value -> num_courses = (((student_element*)student) -> value -> num_courses) - 1;


	/*

	node_t *course = find(&CourseDict, c_key);
	node_t *student = find(&StudentDict, s_key);

	if (course == NULL) {
		fprintf(stderr, "\nCourse does not exist.");
		return;
	}

	//node_t **pptr = &(((course_element*)course) -> value -> list);
	node_t *pptr = (((course_element*)course) -> value -> list);
	node_t **ppptr = &pptr;
	node_t *snode = findList(pptr, s_key);
	if (snode == NULL) {
		fprintf(stderr, "\nStudent is not enrolled in course.");
		return;
	}
	else {
		deleteList(ppptr, snode);
	}

	node_t *pptr2 = (((student_element*)student) -> value -> list);
	node_t **ppptr2 = &pptr2;
	node_t *cnode = findList(pptr2, c_key);
	if (cnode == NULL) {
		fprintf(stderr, "\nStudent is not enrolled in course.");
		return;
	}
	else {
		deleteList(ppptr2, cnode);
	}
	*/

}


// -------------------------------------------
// prints the StudentDict.  For each student,
// list their courses.
// -------------------------------------------
void printStudents()
{
	printf("Students:\n");
	print( &StudentDict );
}

// -------------------------------------------
// prints the CourseDict.  For each course,
// list the students enrolled.
// -------------------------------------------
void printCourses()
{
	printf("Courses:\n");
	print( &CourseDict );
}

// -------------------------------------------
// Don't edit the code below.  This is the driver code to test
// your implementation.
// -------------------------------------------
int main()
{
	char	line[100];
	char 	*command;
	int	done = 0;

        course_element  *ce;
        student_element *se;

	char	*course_key;
	char	*student_key;

	// initialize dictionaries
	create( &CourseDict, 20 );
	create( &StudentDict, 20 );

	// process input
	while( ! done )
	{
		// read an entire line as a string
		fgets(line, 100, stdin);

		// pulls command code, assume comma or tab separated
		command = strtok( line, " \t" );

		if (strcmp(command, "ex")==0)
			{	printf( "Exited Program\n" );
				done = 1;
			}
		else if (strcmp(command, "ac")==0)		// add a new course
			{
				// printf("ac\n");
				
				// Parse and package rest of line into a course_element
				ce = (course_element *) malloc( sizeof(course_element) );
				course_key = strtok( NULL, " " );
				printf( "Adding course: %s\n", course_key );
				memcpy( (*ce).key, course_key, 5 );
				ce->value = (void *) newNode( 'c' );
				ce->value->capacity = atoi( strtok( NULL, " " ) );
				ce->value->num_students = 0;
				ce->value->list = NULL;
				addCourse( ce );
			}
		else if (strcmp(command, "dc")==0)		// delete a course
			{
				course_key = strtok( NULL, " " );
				delCourse(course_key);
				printf("Delete Course Complete\n");
			}
		else if (strcmp(command, "en")==0)		// enroll a student
			{
				// printf("en\n");
				
				// Parse and package rest of line into a course_element
				student_key = strtok( NULL, " " );
				course_key = strtok( NULL, " " );
				printf( "Enrolling student: %s into course: %s\n", student_key, course_key );
				enroll( student_key, course_key );
			}
		else if (strcmp(command, "dr")==0)		// drop a student
			{
				student_key = strtok( NULL, " " );
				course_key = strtok( NULL, " " );
				printf( "Dropping student: %s from course: %s\n", student_key, course_key );
				drop(student_key, course_key);
			}
		else if (strcmp(command, "ps")==0)		// print students
			printStudents();
		else if (strcmp(command, "pc")==0)		// print courses
			printCourses();
		else printf("Unrecognized command: %2s\n", command);


	}

	return 0;
}