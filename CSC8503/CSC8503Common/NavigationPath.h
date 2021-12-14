#pragma once
#include "../../Common/Vector3.h"
#include <vector>

namespace NCL {
	using namespace NCL::Maths;
	namespace CSC8503 {
		class NavigationPath		{
		public:
			NavigationPath() {}
			~NavigationPath() {}

			void Clear() {
				waypoints.clear();
			}
			bool IsEmpty() {
				return waypoints.empty();
			}
			void	PushWaypoint(const Vector3& wp) {
				waypoints.emplace_back(wp);
			}
			bool	PopWaypoint(Vector3& waypoint) {
				if (waypoints.empty()) {
					return false;
				}
				waypoint = waypoints.back();
				waypoints.pop_back();
				return true;
			}

			const std::vector <Vector3>& GetPoints() const { return waypoints; };

		protected:

			std::vector <Vector3> waypoints;
		};
	}
}

