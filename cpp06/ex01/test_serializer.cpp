#include "Data.hpp"
#include "Serializer.hpp"
#include "gtest/gtest.h"
#include <string>

// Test case for the Serializer class
TEST(SerializerTest, SerializeAndDeserialize) {
    // 1. Create and initialize a Data object using your struct members.
    Data original_data;
    original_data.id = 123;
    original_data.type = 'A';
    original_data.content = "Test Content";

    // Store the original pointer.
    Data *original_ptr = &original_data;

    // 2. Serialize the pointer to an integer type.
    uintptr_t raw = Serializer::serialize(original_ptr);

    // 3. Deserialize the integer back to a pointer.
    Data *deserialized_ptr = Serializer::deserialize(raw);

    // 4. Assert that the deserialized pointer is equal to the original pointer.
    // This is the main requirement of the exercise.
    ASSERT_EQ(deserialized_ptr, original_ptr);

    // 5. (Optional but good practice) Assert that the data itself is accessible
    // and correct through the new pointer.
    ASSERT_EQ(deserialized_ptr->id, 123);
    ASSERT_EQ(deserialized_ptr->type, 'A');
    ASSERT_EQ(deserialized_ptr->content, "Test Content");
}

TEST(SerializerTest, NullPointerTest) {
    // Test the behavior with a NULL pointer.
    Data *null_ptr = NULL;

    uintptr_t raw = Serializer::serialize(null_ptr);
    Data *deserialized_ptr = Serializer::deserialize(raw);

    // Serializing and deserializing a NULL pointer should result in a NULL pointer.
    ASSERT_EQ(deserialized_ptr, null_ptr);
}
