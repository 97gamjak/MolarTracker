#ifndef __APP__REPOS__I_REPO_HPP__
#define __APP__REPOS__I_REPO_HPP__

template <typename Domain, typename SQLRow>
class IRepo
{
   public:
    virtual ~IRepo() = default;

    virtual static inline Domain toDomain(const SQLRow& row) = 0;
    virtual static inline SQLRow toRow(const Domain& domain) = 0;
}