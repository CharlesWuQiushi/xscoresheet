#include "rubric.h"
#include <bits/stdc++.h>
using namespace xscoresheet;

int main () {
	std::ofstream fout ("test.xsrubric");
	cereal::PortableBinaryOutputArchive ar (fout);

	rubric g {
		.name = "测试标准",
		.note = "onvqqpwqwc",
		.created = std::chrono::system_clock::now (),
		.modified = std::chrono::system_clock::now (),
		.sports = {
			rubric::sport {
				.name = "立定跳远",
				.unit = "厘米",
				.full_score = 100,
				.comp = rubric::sport::comparator::greater_equal
			}
		},
		.tables = {
			rubric::table {
				.sport_name = "立定跳远",
				.properties = {
					{ "年级", "初三" },
					{ "性别", "男" }
				},
				.entries = {
					{ "180", 40 },
					{ "200", 60 },
					{ "220", 80 },
					{ "250", 100 }
				}
			},
			rubric::table {
				.sport_name = "立定跳远",
				.properties = {
					{ "年级", "初二" },
					{ "性别", "女" }
				},
				.entries = {
					{ "160", 40 },
					{ "180", 60 },
					{ "200", 80 },
					{ "210", 100 }
				},
				.conv = {
					"$M'$S"s,
					"$M*60+$S"s
				}
			}
		}
	};

	ar (g);

	return 0;
}