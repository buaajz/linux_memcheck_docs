#include "tcmalloc/malloc_extension.h"
#include "tcmalloc/internal_malloc_extension.h"
#include "tcmalloc/parameters.h"

#define M_32 33554432

#define ABSL_INTERNAL_HAVE_WEAK_MALLOCEXTENSION_STUBS 1

namespace tcmalloc {
namespace tcmalloc_internal {
// TCMALLOC_PAGE_SHIFT == 13
inline constexpr size_t kPageShift = 13;
inline constexpr size_t kNumBaseClasses = 86;
inline constexpr bool kHasExpandedClasses = true;
inline constexpr size_t kMaxSize = 256 * 1024; // Modified and under testing
inline constexpr size_t kMinThreadCacheSize = kMaxSize * 2;
inline constexpr size_t kMaxThreadCacheSize = 4 << 20;
inline constexpr size_t kMaxCpuCacheSize = 1.5 * 1024 * 1024;
inline constexpr size_t kDefaultOverallThreadCacheSize =
    8u * kMaxThreadCacheSize;
inline constexpr size_t kStealAmount = 1 << 16;
inline constexpr size_t kDefaultProfileSamplingRate = 1 << 21;
inline constexpr size_t kMinPages = 8;

inline constexpr size_t kPageSize = 1 << kPageShift;
}
}
//using tcmalloc::tcmalloc_internal::kPageShift;
//using tcmalloc::tcmalloc_internal::kPageSize;
//using tcmalloc::tcmalloc_internal::Static;

class GuardedAllocAlignment {
 public:
  GuardedAllocAlignment() {
    profile_sampling_rate_ = tcmalloc::MallocExtension::GetProfileSamplingRate();
    guarded_sample_rate_ = tcmalloc::MallocExtension::GetGuardedSamplingRate();
    tcmalloc::MallocExtension::SetProfileSamplingRate(1);  // Always do heapz samples.
    tcmalloc::MallocExtension::SetGuardedSamplingRate(
        0);  // TODO(b/201336703): Guard every heapz sample.
    tcmalloc::MallocExtension::ActivateGuardedSampling();
    // Per-CPU Mode
    if (&TCMalloc_Internal_ForceCpuCacheActivation != nullptr) {
      tcmalloc::tcmalloc_internal::Parameters::set_per_cpu_caches(true);
      TCMalloc_Internal_ForceCpuCacheActivation();
      tcmalloc::MallocExtension::SetMaxPerCpuCacheSize(350UL << 20);
    }
    // Per-Thread Mode
    //tcmalloc::MallocExtension::SetMaxTotalThreadCacheBytes(2048UL << 20);
    //std::cout << "Tcmalloc SamplingRate: " << tcmalloc::MallocExtension::GetProfileSamplingRate() << ", " << profile_sampling_rate_  << std::endl;
    //std::cout << "Tcmalloc PerCpu Mode: " << tcmalloc::MallocExtension::PerCpuCachesActive() << std::endl;
    //std::cout << "Tcmalloc ThreadCache size: " << tcmalloc::MallocExtension::GetMaxTotalThreadCacheBytes() << std::endl;
    std::cout << "Tcmalloc KMaxSize size: " << tcmalloc::tcmalloc_internal::kMaxSize << std::endl;
    //std::cout << "Tcmalloc CpuCache size: " << tcmalloc::MallocExtension::GetMaxPerCpuCacheSize() << std::endl;
    // Eat up unsampled bytes remaining to flush the new sample rates.

    //int i = 16;
    //while (i--) {
    //  void* p = ::operator new(tcmalloc::tcmalloc_internal::kPageSize);
      //if (tcmalloc::Static::guardedpage_allocator().PointerIsMine(p)) {
      //  ::operator delete(p);
      //  break;
      //}
      //::operator delete(p);
    //}

    // Ensure subsequent allocations are guarded.
    //void* p = ::operator new(1);
    //CHECK_CONDITION(tcmalloc::Static::guardedpage_allocator().PointerIsMine(p));
    //::operator delete(p);
  }

  ~GuardedAllocAlignment() {
    tcmalloc::MallocExtension::SetProfileSamplingRate(profile_sampling_rate_);
    tcmalloc::MallocExtension::SetGuardedSamplingRate(guarded_sample_rate_);
  }

 private:
  int64_t profile_sampling_rate_;
  int32_t guarded_sample_rate_;
};

class PreInitializer {
public:
    PreInitializer() {
        // std::cout << "Pre-initialization code executed." << std::endl;
        // 在这里可以执行你想要在加载前运行的代码
        // tcmalloc::MallocExtension::SetGuardedSamplingRate(128);// 8M
                // tcmalloc::MallocExtension::ActivateGuardedSampling();
                GuardedAllocAlignment new_test;
    }
};

// 在全局范围内创建一个静态对象，其构造函数将在程序加载前执行
static PreInitializer preInitializer;

