#pragma once
#include "../essential/object.h"

class testComponent {
public:
	testComponent() = default;

	testComponent(glg::Object& obj) {

	}

	virtual testComponent& operator=(const testComponent& other) {
		return *this;
	}
};