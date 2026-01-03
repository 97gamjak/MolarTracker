#ifndef __APP__REPOS__I_REPO_HPP__
#define __APP__REPOS__I_REPO_HPP__

template <typename Domain, typename SQLRow>
class IRepo
{
   public:
    virtual ~IRepo() = default;

    virtual inline Domain toDomain(const SQLRow& row) const = 0;
    virtual inline SQLRow toRow(const Domain& domain) const = 0;
};

#endif   // __APP__REPOS__I_REPO_HPP__