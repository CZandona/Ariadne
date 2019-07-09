/***************************************************************************
 *            function.cpp
 *
 *  Copyright 2008--17  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../numeric/numeric.hpp"

#include "../numeric/operators.hpp"
#include "../algebra/differential.hpp"
#include "../algebra/algebra.hpp"
#include "../function/formula.hpp"
#include "../function/taylor_model.hpp"

#include "../function/function.hpp"

#include "../function/function_mixin.hpp"
#include "../function/function_mixin.tpl.hpp"

#include "../function/function_model.hpp"

#include "../function/symbolic_function.hpp"

#include "../symbolic/expression.hpp"


namespace Ariadne {

template<class T> inline String class_name() { return "Unknown"; }

template<class T> inline StringType str(const T& t) {
    StringStream ss; ss << t; return ss.str(); }

// Templated conversions dynamically checked at runtime
template<class R, class A, EnableIf<IsSame<R,A>> =dummy> R const& checked_same(A const& a) { return a; }
template<class R, class A, DisableIf<IsSame<R,A>> =dummy> R const& checked_same(A const& a) {
    ARIADNE_THROW(std::runtime_error,"checked_same<R,A> with R="<<class_name<R>()<<", A="<<class_name<A>(),"argument "<<a<<" does not have the same type as result."); }
template<class R, class A, EnableIf<IsConvertible<A,R>> =dummy> R checked_convert(A&& a) { return a; }
template<class R, class A, DisableIf<IsConvertible<A,R>> =dummy> R checked_convert(A&& a) {
    ARIADNE_THROW(std::runtime_error,"checked_convert<R,A> with R="<<class_name<R>()<<", A="<<class_name<A>(),"argument "<<a<<" is not convertible to result."); }
template<class R, class A, EnableIf<IsConstructible<R,A>> =dummy> R checked_construct(A const& a) { return R(a); }
template<class R, class A, DisableIf<IsConstructible<R,A>> =dummy> R checked_construct(A const& a) {
    ARIADNE_THROW(std::runtime_error,"checked_construct<R,A> with R="<<class_name<R>()<<", A="<<class_name<A>(),"argument "<<a<<" is not explicitly convertible to result."); }



//------------------------ Formula Function ----------------------------------//

template<class P, class Y> ScalarFunction<P,IntervalDomainType> make_formula_function(IntervalDomainType dom, Scalar<Formula<Y>> const& e) {
    assert(false); return ScalarFunction<P,IntervalDomainType>(nullptr);
}

template<class P, class Y> VectorFunction<P,IntervalDomainType> make_formula_function(IntervalDomainType dom, Vector<Formula<Y>> const& e) {
    assert(false); return VectorFunction<P,IntervalDomainType>(nullptr);
}

template<class P, class Y> ScalarFunction<P,BoxDomainType> make_formula_function(BoxDomainType dom, Scalar<Formula<Y>> const& e) {
    return ScalarFunction<P,BoxDomainType>(new ScalarFormulaFunction<Y>(dom.dimension(),e));
}

template<class P, class Y> VectorFunction<P,BoxDomainType> make_formula_function(BoxDomainType dom, Vector<Formula<Y>> const& e) {
    return VectorFunction<P,BoxDomainType>(new VectorFormulaFunction<Y>(dom.dimension(),e));
}

//------------------------ Function ----------------------------------//

namespace {

template<class D, class DD> D make_domain(DD dom);
template<> IntervalDomainType make_domain<IntervalDomainType,BoxDomainType>(BoxDomainType dom) { throw std::runtime_error(""); }
template<> IntervalDomainType make_domain<IntervalDomainType,IntervalDomainType>(IntervalDomainType dom) { return dom; }
template<> BoxDomainType make_domain<BoxDomainType,BoxDomainType>(BoxDomainType dom) { return dom; }

template<class P, class D, class DD> ScalarFunction<P,D> make_zero_function(SizeOne rs, DD dom) {
    return FunctionConstructors<P>::zero(make_domain<D>(dom)); }
template<class P, class D, class DD> VectorFunction<P,D> make_zero_function(SizeType rs, DD dom) {
    return  FunctionConstructors<P>::zeros(rs,make_domain<D>(dom)); }
}

template<class P, class D, class C> Function<P,D,C>::Function() : _ptr() {
}

template<class P, class D, class C> Function<P,D,C>::Function(EuclideanDomain dom) {
    ResultSizeType rs=ResultSizeType(); BoxDomainType bx_dom=dom;
    (*this) = make_zero_function<P,D>(rs,bx_dom);
}

template<class P, class D, class C> Function<P,D,C>::Function(ResultSizeType rs, EuclideanDomain dom) {
    BoxDomainType const& bx_dom=dom;
    (*this) = make_zero_function<P,D>(rs,bx_dom);
}

template<class P, class D, class C> Function<P,D,C>::Function(DomainType dom) {
    ResultSizeType rs=ResultSizeType(); (*this) = make_zero_function<P,D>(rs,dom);
}

template<class P, class D, class C> Function<P,D,C>::Function(ResultSizeType rs, DomainType dom) {
    (*this) = make_zero_function<P,D>(rs,dom);
}

template<class P, class D, class C> Function<P,D,C>::Function(ResultSizeType rs, ScalarFunction<P,D> sf)
    : Function(Vector<ScalarFunction<P,D>>(SizeType(rs),sf)) {
}

template<class P, class D, class C> Function<P,D,C>::Function(InitializerList<ScalarFunction<P,D>> const& lsf)
    : Function(Vector<ScalarFunction<P,D>>(lsf)) {
}

template<class P, class D, class C> Function<P,D,C>::Function(List<ScalarFunction<P,D>> const& lsf)
    : Function(Vector<ScalarFunction<P,D>>(lsf)) {
}

template<class P, class D, class C> Function<P,D,C>::Function(DomainType dom, Result<Formula<Y>>const& e) {
    *this = make_formula_function<P>(dom,e);
}

template<class P, class D, class C> struct MakeVectorMultivariateFunction;
template<class P, class D> struct MakeVectorMultivariateFunction<P,D,IntervalDomainType> {
    Function<P,D,IntervalDomainType> create(Vector<ScalarFunction<P,D>> const& lsf) {
        ARIADNE_FAIL_MSG("Cannot construct scalar function from list."); }
};
template<class P, class D> struct MakeVectorMultivariateFunction<P,D,BoxDomainType> {
    Function<P,D,BoxDomainType> create(Vector<ScalarFunction<P,D>> const& lsf) {
        return Function<P,D,BoxDomainType>(std::make_shared<VectorOfScalarFunction<P,D>>(lsf)); }
};

template<class P, class D, class C> Function<P,D,C> make_vector_function(Vector<ScalarFunction<P,D>> const& lsf) {
    return MakeVectorMultivariateFunction<P,D,C>().create(lsf);
}

template<class P, class D, class C> Function<P,D,C>::Function(Vector<ScalarFunction<P,D>> const& vsf)
    : Function<P,D,C>(make_vector_function<P,D,C>(vsf)) {
}


//------------------------ Function Constructors -----------------------------------//

template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::zero(BoxDomainType dom) {
    return ConstantFunction<Y,BoxDomainType>(dom, Y(0));
}


template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::constant(BoxDomainType dom, NumericType c) {
    return ConstantFunction<Y,BoxDomainType>(dom, c);
}

template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::coordinate(BoxDomainType dom, SizeType j) {
    return CoordinateFunction<P,BoxDomainType>(dom, j);
}

template<class P> List<ScalarMultivariateFunction<P>> FunctionConstructors<P>::coordinates(BoxDomainType dom) {
    List<ScalarMultivariateFunction<P>> r; r.reserve(dom.dimension());
    for(SizeType j=0; j!=dom.dimension(); ++j) { r.append(coordinate(dom,j)); }
    return std::move(r);
}

template<class P> VectorMultivariateFunction<P> FunctionConstructors<P>::zeros(SizeType rs, BoxDomainType dom) {
    return VectorMultivariateFunction<P>(new VectorOfScalarMultivariateFunction<P>(rs,zero(dom)));
}

template<class P> VectorMultivariateFunction<P> FunctionConstructors<P>::identity(BoxDomainType dom) {
    SizeType n=dom.dimension();
    ScalarMultivariateFunction<P> z=ScalarMultivariateFunction<P>::zero(dom);
    VectorOfScalarMultivariateFunction<P>* res = new VectorOfScalarMultivariateFunction<P>(n,z);
    for(SizeType i=0; i!=n; ++i) {
        res->_vec[i]=ScalarMultivariateFunction<P>::coordinate(dom,i);
    }
    return VectorMultivariateFunction<P>(res);
}


template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::zero(IntervalDomainType dom) {
    return ConstantFunction<Y,IntervalDomainType>(dom, Y(0));
}

template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::constant(IntervalDomainType dom, NumericType c) {
    return ConstantFunction<Y,IntervalDomainType>(dom,c);
}

template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::coordinate(IntervalDomainType dom, SizeOne as) {
    return CoordinateFunction<P,IntervalDomainType>(dom,as);
}

template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::coordinate(IntervalDomainType dom) {
    return coordinate(dom,SizeOne());
}

template<class P> VectorUnivariateFunction<P> FunctionConstructors<P>::zeros(SizeType rs, IntervalDomainType dom) {
    return constant(dom,Vector<Y>(rs,0));
}


template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::identity(IntervalDomainType dom) {
    return coordinate(dom,SizeOne());
}


template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::zero() {
    return zero(IntervalDomainType::biinfinite_interval());
}

template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::constant(NumericType c) {
    return constant(IntervalDomainType::biinfinite_interval(),c);
}

template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::coordinate() {
    return coordinate(IntervalDomainType::biinfinite_interval());
}

template<class P> VectorUnivariateFunction<P> FunctionConstructors<P>::zeros(SizeType rs) {
    return zeros(rs,IntervalDomainType::biinfinite_interval());
}


template<class P> ScalarUnivariateFunction<P> FunctionConstructors<P>::identity() {
    return identity(IntervalDomainType::biinfinite_interval());
}


template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::zero(SizeType as) {
    ScalarMultivariateFunction<P> sf(new ScalarFormulaFunction<Y>(as,Formula<Y>::zero()));
    return sf;
}

template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::constant(SizeType as, NumericType c) {
    return ScalarMultivariateFunction<P>(new ScalarFormulaFunction<Y>(as,Formula<Y>::constant(c)));
}

template<class P> ScalarMultivariateFunction<P> FunctionConstructors<P>::coordinate(SizeType as, SizeType j) {
    ARIADNE_ASSERT(j<as);
    return ScalarMultivariateFunction<P>(new ScalarFormulaFunction<Y>(as,Formula<Y>::coordinate(j)));
}

template<class P> VectorMultivariateFunction<P> FunctionConstructors<P>::zeros(SizeType rs, SizeType as) {
    VectorOfScalarFunction<P,BoxDomainType>* res = new VectorOfScalarFunction<P,BoxDomainType>(rs,as);
    for(SizeType i=0; i!=rs; ++i) {
        res->_vec[i]=ScalarMultivariateFunction<P>::zero(as);
    }
    return VectorMultivariateFunction<P>(res);
}

template<class P> VectorMultivariateFunction<P> FunctionConstructors<P>::constant(SizeType as, Vector<NumericType> c) {
    SizeType rs=c.size();
    VectorOfScalarFunction<P,BoxDomainType>* res = new VectorOfScalarFunction<P,BoxDomainType>(rs,as);
    for(SizeType i=0; i!=rs; ++i) {
        res->_vec[i]=ScalarMultivariateFunction<P>::constant(as,c[i]);
    }
    return VectorMultivariateFunction<P>(res);
}

template<class P> List<ScalarMultivariateFunction<P>> FunctionConstructors<P>::coordinates(SizeType as) {
    List<ScalarMultivariateFunction<P>> r; r.reserve(as);
    for(SizeType j=0; j!=as; ++j) { r.append(coordinate(as,j)); }
    return std::move(r);
}

template<class P> VectorMultivariateFunction<P> FunctionConstructors<P>::identity(SizeType n) {
    ScalarMultivariateFunction<P> z=ScalarMultivariateFunction<P>::zero(n);
    VectorOfScalarFunction<P,BoxDomainType>* res = new VectorOfScalarFunction<P,BoxDomainType>(n,n);
    for(SizeType i=0; i!=n; ++i) {
        res->_vec[i]=ScalarMultivariateFunction<P>::coordinate(n,i);
    }
    return VectorMultivariateFunction<P>(res);
}

template<class P> VectorFunction<P,BoxDomainType> FunctionConstructors<P>::constant(BoxDomainType dom, Vector<NumericType> c) {
    SizeType n=c.size();
    ScalarFunction<P,BoxDomainType> z=ScalarFunction<P,BoxDomainType>::zero(dom);
    VectorOfScalarFunction<P,BoxDomainType>* res = new VectorOfScalarFunction<P,BoxDomainType>(n,z);
    for(SizeType i=0; i!=n; ++i) {
        res->_vec[i]=ScalarMultivariateFunction<P>::constant(dom,c[i]);
    }
    return VectorMultivariateFunction<P>(res);
}

template<class P> VectorFunction<P,IntervalDomainType> FunctionConstructors<P>::constant(IntervalDomainType dom, Vector<NumericType> c) {
    SizeType n=c.size();
    ScalarFunction<P,IntervalDomainType> z=ScalarFunction<P,IntervalDomainType>::zero(dom);
    VectorOfScalarFunction<P,IntervalDomainType>* res = new VectorOfScalarFunction<P,IntervalDomainType>(n,z);
    for(SizeType i=0; i!=n; ++i) {
        res->_vec[i]=ScalarFunction<P,IntervalDomainType>::constant(dom,c[i]);
    }
    return VectorFunction<P,IntervalDomainType>(res);
}

template<class P> VectorFunction<P,IntervalDomainType> FunctionConstructors<P>::constant(Vector<NumericType> c) {
    return constant(IntervalDomainType::biinfinite_interval(),c);
}


template class FunctionConstructors<ApproximateTag>;
template class FunctionConstructors<ValidatedTag>;
template class FunctionConstructors<EffectiveTag>;

//------------------------ Converting to and from Expression classes to Function classes via Formula -----------------------------------//

template<class X> class Expression;
template<class X> class Variable;
template<class X> class Space;

SizeType dimension(const Space<Real>& spc);


inline EffectiveScalarUnivariateFunction make_formula_function(SizeOne as, const Formula<EffectiveNumber>& fm) {
    return EffectiveScalarUnivariateFunction(RealDomain(),fm); }
inline EffectiveScalarMultivariateFunction make_formula_function(SizeType as, const Formula<EffectiveNumber>& fm) {
    return EffectiveScalarMultivariateFunction(EuclideanDomain(as),fm); }
inline EffectiveVectorMultivariateFunction make_formula_function(SizeType as, const Vector<Formula<EffectiveNumber>>& fm) {
    return EffectiveVectorMultivariateFunction(EuclideanDomain(as),fm); }

EffectiveScalarUnivariateFunction make_function(const Variable<Real>& var, const Expression<Real>& expr) {
    return make_formula_function(SizeOne(),make_formula(expr,var)); }
EffectiveScalarMultivariateFunction make_function(const Space<Real>& spc, const Expression<Real>& expr) {
    return make_formula_function(dimension(spc),make_formula(expr,spc)); }
EffectiveVectorMultivariateFunction make_function(const Space<Real>& spc, const Vector<Expression<Real>>& expr) {
    return make_formula_function(dimension(spc),make_formula(expr,spc)); }

template<class P, class D, class C, class E> Function<P,D,C> make_composed_function(const Function<P,E,C>& f, const Function<P,D,E>& g) {
    ARIADNE_ASSERT(f.argument_size()==g.result_size());
    return Function<P,D,C>(new ComposedFunction<P,D,C,E>(f,g)); }

[[deprecated]]
EffectiveScalarMultivariateFunction make_function(const Expression<Real>& expr, const Space<Real>& spc) {
    return make_function(spc,expr); }

Formula<Real> make_formula(const EffectiveScalarMultivariateFunction& f) {
    Vector<Formula<Real>> x(f.argument_size());
    for(SizeType i=0; i!=x.size(); ++i) {
        x[i]=Formula<Real>::coordinate(i);
    }
    return f(x);
}

Vector<Formula<Real>> make_formula(const EffectiveVectorMultivariateFunction& f) {
    const VectorMultivariateFunctionInterface<EffectiveTag>& fi=f;
    const VectorFormulaFunction<Real>* ff;
    const VectorOfScalarFunction<EffectiveTag,BoxDomainType>* vf;
    if( (vf=dynamic_cast<const VectorOfScalarFunction<EffectiveTag,BoxDomainType>*>(&fi)) ) {
        Vector< Formula<Real> > r(vf->result_size());
        for(SizeType i=0; i!=r.size(); ++i) { r[i]=make_formula((*vf)[i]); }
        return r;
    } else if( (ff=dynamic_cast<const VectorFormulaFunction<Real>*>(&fi)) ) {
        return ff->_formulae;
    } else {
        ARIADNE_FAIL_MSG("Cannot compute formula for function "<<f<<"\n");
    }
}

//------------------------ Scalar Function ----------------------------------//

//------------------------ Vector Function ----------------------------------//

template<class P, class D, class C> ScalarFunction<P,D> Function<P,D,C>::get(SizeType i) const {
    ARIADNE_ASSERT((IsSame<ResultSizeType,SizeType>::value));
    const VectorOfFunctionInterface<P,D>* vfp = dynamic_cast<const VectorOfFunctionInterface<P,D>*>(this->raw_pointer());
    if(!vfp) { std::cerr<<"\nCannot get element of "<<*this<<"\n  of type "<<typeid(this->raw_pointer()).name()<<":"<<typeid(this->reference()).name()<<"\n\n"; }
    return ScalarFunction<P,D>(SharedPointer<ScalarFunctionInterface<P,D>>(vfp->_get(i)));
}

template<class P, class D, class C> Void Function<P,D,C>::set(SizeType i, ScalarFunction<P,D> sf) {
    ARIADNE_ASSERT((IsSame<ResultSizeType,SizeType>::value));
    const VectorOfScalarFunction<P,D>& cvf = dynamic_cast<const VectorOfScalarFunction<P,D>&>(this->_ptr.operator*());
    VectorOfScalarFunction<P,D>& vf = const_cast<VectorOfScalarFunction<P,D>&>(cvf);
    vf[i]=sf;
}

/*
template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction()
    : Function<P,D,C>(new VectorOfScalarFunction<P,D>(0u,ScalarFunction<P,D>()))
{
}

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(SizeType rs, SizeType as)
    : Function<P,D,C>(new VectorOfScalarFunction<P,D>(rs,ScalarFunction<P,D>::zero(as)))
{
}

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(const InitializerList<ScalarFunction<P,D>>& lsf)
    : VectorFunction<P,D>(List<ScalarFunction<P,D>>(lsf)) { }

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(const List<ScalarFunction<P,D>>& lsf) {
    ARIADNE_ASSERT(lsf.size()>0);
    SizeType as=lsf[0].argument_size();
    VectorOfScalarFunction<P,D>* new_ptr=new VectorOfScalarFunction<P,D>(lsf.size(),as);
    for(SizeType i=0; i!=lsf.size(); ++i) {
        new_ptr->set(i,lsf[i]);
    }
    this->_ptr=std::shared_ptr< const VectorFunctionInterface<P,D> >(new_ptr);
}

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(const Vector<ScalarFunction<P,D>>& vsf) {
    VectorOfScalarFunction<P,D>* new_ptr=new VectorOfScalarFunction<P,D>(vsf);
    this->_ptr=std::shared_ptr< const VectorFunctionInterface<P,D> >(new_ptr);
}

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(SizeType as, const List<Formula<X>>& le) {
    ARIADNE_ASSERT(le.size()>0);
    VectorOfScalarFunction<P,D>* new_ptr=new VectorOfScalarFunction<P,D>(le.size(),as);
    for(SizeType i=0; i!=le.size(); ++i) {
        new_ptr->set(i,ScalarFormulaFunction<X>(as,le[i]));
    }
    this->_ptr=std::shared_ptr< const VectorFunctionInterface<P,D> >(new_ptr);
}

template<class P, class D> VectorFunction<P,D>::VectorMultivariateFunction(SizeType rs, ScalarFunction<P,D> const& sf)
    : Function<P,D,C>(new VectorOfScalarFunction<P,D>(rs,sf))
{
}


template<class P, class D> ScalarFunction<P,D> VectorFunction<P,D>::get(SizeType i) const {
    VectorOfFunctionInterface<P,D> const* vfi=dynamic_cast<VectorOfFunctionInterface<P,D>const*>(this->raw_pointer());
    ARIADNE_PRECONDITION(vfi);
    return vfi->_get(i);
}

template<class P, class D> Void VectorFunction<P,D>::set(SizeType i, ScalarFunction<P,D> const& sf) {
    const VectorOfFunctionInterface<P,D>* cvfi=dynamic_cast<const VectorOfFunctionInterface<P,D>*>(this->_ptr.operator->());
    VectorOfFunctionInterface<P,D>* vfi=const_cast<VectorOfFunctionInterface<P,D>*>(cvfi);
    ARIADNE_PRECONDITION(vfi);
    vfi->_set(i,sf);
}

template class VectorMultivariateFunction<ApproximateTag>;
template class VectorMultivariateFunction<ValidatedTag>;
template class VectorMultivariateFunction<EffectiveTag>;

*/

//------------------------ Instantiate functions -----------------------------------//

template class Function<ApproximateTag,IntervalDomainType,IntervalDomainType>;
template class Function<ApproximateTag,IntervalDomainType,BoxDomainType>;
template class Function<ApproximateTag,BoxDomainType,IntervalDomainType>;
template class Function<ApproximateTag,BoxDomainType,BoxDomainType>;

template class Function<ValidatedTag,IntervalDomainType,IntervalDomainType>;
template class Function<ValidatedTag,IntervalDomainType,BoxDomainType>;
template class Function<ValidatedTag,BoxDomainType,IntervalDomainType>;
template class Function<ValidatedTag,BoxDomainType,BoxDomainType>;

template class Function<EffectiveTag,IntervalDomainType,IntervalDomainType>;
template class Function<EffectiveTag,IntervalDomainType,BoxDomainType>;
template class Function<EffectiveTag,BoxDomainType,IntervalDomainType>;
template class Function<EffectiveTag,BoxDomainType,BoxDomainType>;





//------------------------ Vector function operators -------------------------------//

EffectiveVectorMultivariateFunction operator*(const EffectiveScalarMultivariateFunction& f, const Vector<EffectiveNumber>& e) {
    for(SizeType i=0; i!=e.size(); ++i) { ARIADNE_ASSERT(decide(e[i]==EffectiveNumber(0)) || decide(e[i]==EffectiveNumber(1))); }
    VectorMultivariateFunction<EffectiveTag> r(e.size(),f.domain());
    for(SizeType i=0; i!=e.size(); ++i) {
        if(decide(e[i]==EffectiveNumber(1))) { r.set(i,f); }
    }
    return r;
}

EffectiveVectorMultivariateFunction operator+(const EffectiveVectorMultivariateFunction& f) {
    EffectiveVectorMultivariateFunction r(f.result_size(),f.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,+f[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator-(const EffectiveVectorMultivariateFunction& f) {
    EffectiveVectorMultivariateFunction r(f.result_size(),f.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,-f[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator+(const EffectiveVectorMultivariateFunction& f1, const EffectiveVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.result_size()==f2.result_size());
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(f1.result_size(),f1.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,f1[i]+f2[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator-(const EffectiveVectorMultivariateFunction& f1, const EffectiveVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.result_size()==f2.result_size());
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(f1.result_size(),f1.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,f1[i]-f2[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator*(const EffectiveScalarMultivariateFunction& sf, const EffectiveVectorMultivariateFunction& vf) {
    ARIADNE_ASSERT(sf.argument_size()==vf.argument_size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,sf*vf[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator*(const EffectiveVectorMultivariateFunction& vf, const EffectiveScalarMultivariateFunction& sf) {
    ARIADNE_ASSERT(vf.argument_size()==sf.argument_size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]*sf);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator/(const EffectiveVectorMultivariateFunction& vf, const EffectiveScalarMultivariateFunction& sf) {
    ARIADNE_ASSERT(vf.argument_size()==sf.argument_size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]/sf);
    }
    return r;
}


EffectiveVectorMultivariateFunction operator+(const Vector<EffectiveNumber>& vc, const EffectiveVectorMultivariateFunction& vf) {
    ARIADNE_ASSERT(vc.size()==vf.result_size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vc[i]+vf[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator+(const EffectiveVectorMultivariateFunction& vf, const Vector<EffectiveNumber>& vc) {
    ARIADNE_ASSERT(vf.result_size()==vc.size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]+vc[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator-(const Vector<EffectiveNumber>& vc, const EffectiveVectorMultivariateFunction& vf) {
    ARIADNE_ASSERT(vc.size()==vf.result_size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vc[i]-vf[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator-(const EffectiveVectorMultivariateFunction& vf, const Vector<EffectiveNumber>& vc) {
    ARIADNE_ASSERT(vf.result_size()==vc.size());
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]-vc[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator*(const EffectiveNumber& c, const EffectiveVectorMultivariateFunction& vf) {
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,c*vf[i]);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator*(const EffectiveVectorMultivariateFunction& vf, const EffectiveNumber& c) {
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]*c);
    }
    return r;
}

EffectiveVectorMultivariateFunction operator/(const EffectiveVectorMultivariateFunction& vf, const EffectiveNumber& c) {
    EffectiveVectorMultivariateFunction r(vf.result_size(),vf.domain());
    for(SizeType i=0; i!=r.result_size(); ++i) {
        r.set(i,vf[i]/c);
    }
    return r;
}



EffectiveVectorMultivariateFunction join(const EffectiveScalarMultivariateFunction& f1, const EffectiveScalarMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(2,f1.domain());
    r.set(0,f1);
    r.set(1,f2);
    return r;
}

EffectiveVectorMultivariateFunction join(const EffectiveVectorMultivariateFunction& f1, const EffectiveScalarMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(f1.result_size()+1u,f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    r.set(f1.result_size(),f2);
    return r;
}

EffectiveVectorMultivariateFunction join(const EffectiveScalarMultivariateFunction& f1, const EffectiveVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(f2.result_size()+1u,f1.domain());
    r.set(0u,f1);
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+1u,f2.get(i));
    }
    return r;
}

EffectiveVectorMultivariateFunction join(const EffectiveVectorMultivariateFunction& f1, const EffectiveVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    EffectiveVectorMultivariateFunction r(f1.result_size()+f2.result_size(),f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+f1.result_size(),f2.get(i));
    }
    return r;
}


EffectiveScalarMultivariateFunction embed(SizeType as1, const EffectiveScalarMultivariateFunction& f2, SizeType as3) {
    typedef BoxDomainType D; typedef IntervalDomainType C;
    return EffectiveScalarMultivariateFunction(new EmbeddedFunction<EffectiveTag,D,D,D,C>(as1,f2,as3));
}

EffectiveVectorMultivariateFunction embed(SizeType as1, const EffectiveVectorMultivariateFunction& f2, SizeType as3) {
    typedef BoxDomainType D; typedef BoxDomainType C;
    return EffectiveVectorMultivariateFunction(new EmbeddedFunction<EffectiveTag,D,D,D,C>(as1,f2,as3));
}

EffectiveScalarMultivariateFunction compose(const EffectiveScalarMultivariateFunction& f, const EffectiveVectorMultivariateFunction& g) {
    return make_composed_function(f,g);
}

EffectiveVectorMultivariateFunction compose(const EffectiveVectorMultivariateFunction& f, const EffectiveVectorMultivariateFunction& g) {
    return make_composed_function(f,g);
}

EffectiveScalarMultivariateFunction lie_derivative(const EffectiveScalarMultivariateFunction& g, const EffectiveVectorMultivariateFunction& f) {
    ARIADNE_ASSERT_MSG(g.argument_size()==f.result_size(),"f="<<f<<", g="<<g<<"\n");
    ARIADNE_ASSERT_MSG(f.result_size()==f.argument_size(),"f="<<f<<", g="<<g<<"\n");
    ARIADNE_ASSERT_MSG(f.result_size()>0,"f="<<f<<", g="<<g<<"\n");

    try {
        EffectiveScalarMultivariateFunction r=g.derivative(0)*f[0];
        for(SizeType i=1; i!=g.argument_size(); ++i) {
            r=r+g.derivative(i)*f[i];
        }
        return r;
    }
    catch(...) {
        ARIADNE_FAIL_MSG("Failed to compute Lie derivative of "<<g<<" under vector field "<<f<<"\n");
    }
}

EffectiveVectorMultivariateFunction lie_derivative(const EffectiveVectorMultivariateFunction& g, const EffectiveVectorMultivariateFunction& f) {
    ARIADNE_ASSERT_MSG(g.argument_size()==f.result_size(),"f="<<f<<", g="<<g<<"\n");
    ARIADNE_ASSERT_MSG(f.result_size()==f.argument_size(),"f="<<f<<", g="<<g<<"\n");
    ARIADNE_ASSERT_MSG(f.result_size()>0,"f="<<f<<", g="<<g<<"\n");

    try {
        EffectiveVectorMultivariateFunction r(g.result_size(),g.domain());
        for(SizeType i=0; i!=g.result_size(); ++i) { r[i]=lie_derivative(g[i],f); }
        return r;
    }
    catch(...) {
        ARIADNE_FAIL_MSG("Failed to compute Lie derivative of vector function "<<g<<" under vector field "<<f<<"\n");
    }
}



//------------------------ Validated function operators -------------------------------//



ValidatedVectorMultivariateFunction operator-(ValidatedVectorMultivariateFunction const& f1, ValidatedVectorMultivariateFunction const& f2) {
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const> f1p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const> f2p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
    if(f1p && f2p) {
        return ValidatedVectorMultivariateFunctionModelDP(*f1p) - ValidatedVectorMultivariateFunctionModelDP(*f2p);
    } else if(f1p) {
        return ValidatedVectorMultivariateFunctionModelDP(*f1p) - f2.reference();
    } else if(f2p) {
        return f1.reference() - ValidatedVectorMultivariateFunctionModelDP(*f2p);
    } else {
        VectorOfScalarFunction<ValidatedTag,BoxDomainType> r(f1.result_size(),ValidatedScalarMultivariateFunction(f1.argument_size()));
        for(SizeType i=0; i!=r.result_size(); ++i) {
            r[i]=f1[i]-f2[i];
        }
        return r;
    }
}


ValidatedScalarMultivariateFunction compose(const ValidatedScalarMultivariateFunction& f, const ValidatedVectorMultivariateFunction& g) {
    ARIADNE_ASSERT(f.argument_size()==g.result_size());
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        gp=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(g.managed_pointer());
    if(gp) {
        return compose(f,ValidatedVectorMultivariateFunctionModelDP(gp->_clone()));
    } else {
        return make_composed_function(f,g);
    }
}

ValidatedVectorMultivariateFunction compose(const ValidatedVectorMultivariateFunction& f, const ValidatedVectorMultivariateFunction& g) {
    ARIADNE_ASSERT(f.argument_size()==g.result_size());
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        gp=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(g.managed_pointer());
    if(gp) {
        return compose(f,ValidatedVectorMultivariateFunctionModelDP(gp->_clone()));
    } else {
        return make_composed_function(f,g);
    }
}

ValidatedVectorMultivariateFunction join(ValidatedVectorMultivariateFunction const& f1, const ValidatedVectorMultivariateFunction& f2) {
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        f1p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        f2p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
    if(f1p && f2p) {
        ValidatedVectorMultivariateFunctionModelDP f1m(f1p); ValidatedVectorMultivariateFunctionModelDP f2m(f2p); return join(f1m,f2m);
    } else if(f1p) {
        ValidatedVectorMultivariateFunctionModelDP f1m(f1p); ValidatedVectorMultivariateFunctionModelDP f2m=factory(f1m).create(f2); return join(f1m,f2m);
    } else if(f2p) {
        ValidatedVectorMultivariateFunctionModelDP f2m(f2p); ValidatedVectorMultivariateFunctionModelDP f1m=factory(f2m).create(f1); return join(f1m,f2m);
    } else {
        typedef ValidatedVectorMultivariateFunction::DomainType DomainType;
        typedef ValidatedVectorMultivariateFunction::CodomainType CodomainType;
        return ValidatedVectorMultivariateFunction(new JoinedFunction<ValidatedTag,DomainType,CodomainType,CodomainType>(f1,f2));
    }
}

ValidatedVectorMultivariateFunction join(ValidatedVectorMultivariateFunction const& f1, const ValidatedScalarMultivariateFunction& f2) {
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        f1p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
    std::shared_ptr<ValidatedScalarMultivariateFunctionModelDPInterface const>
        f2p=std::dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
    if(f1p && f2p) {
        ValidatedVectorMultivariateFunctionModelDP f1m(f1p); ValidatedScalarMultivariateFunctionModelDP f2m(f2p); return join(f1m,f2m);
    } else if(f1p) {
        ValidatedVectorMultivariateFunctionModelDP f1m(f1p); ValidatedScalarMultivariateFunctionModelDP f2m=factory(f1m).create(f2); return join(f1m,f2m);
    } else if(f2p) {
        ValidatedScalarMultivariateFunctionModelDP f2m(f2p); ValidatedVectorMultivariateFunctionModelDP f1m=factory(f2m).create(f1); return join(f1m,f2m);
    } else {
        VectorOfScalarFunction<ValidatedTag,BoxDomainType> r(f1.result_size()+1u,f1.domain());
        for(SizeType i=0; i!=f1.result_size(); ++i) { r[i]=f1[i]; }
        r[f1.result_size()]=f2;
        return r;
    }
}

ValidatedVectorMultivariateFunction join(ValidatedScalarMultivariateFunction const& f1, const ValidatedVectorMultivariateFunction& f2) {
    std::shared_ptr<ValidatedScalarMultivariateFunctionModelDPInterface const>
        f1p=std::dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
    std::shared_ptr<ValidatedVectorMultivariateFunctionModelDPInterface const>
        f2p=std::dynamic_pointer_cast<ValidatedVectorMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
    if(f1p && f2p) {
        ValidatedScalarMultivariateFunctionModelDP f1m(f1p); ValidatedVectorMultivariateFunctionModelDP f2m(f2p); return join(f1m,f2m);
    } else if(f1p) {
        ValidatedScalarMultivariateFunctionModelDP f1m(f1p); ValidatedVectorMultivariateFunctionModelDP f2m=factory(f1m).create(f2); return join(f1m,f2m);
    } else if(f2p) {
        ValidatedVectorMultivariateFunctionModelDP f2m(f2p); ValidatedScalarMultivariateFunctionModelDP f1m=factory(f2m).create(f1); return join(f1m,f2m);
    } else {
        VectorOfScalarFunction<ValidatedTag,BoxDomainType> r(f1.result_size()+1u,f1.domain());
        r[0u]=f1;
        for(SizeType i=0; i!=f1.result_size(); ++i) { r[i+1]=f2[i]; }
        return r;
    }
}

ValidatedVectorMultivariateFunction join(ValidatedScalarMultivariateFunction const& f1, const ValidatedScalarMultivariateFunction& f2) {
    std::shared_ptr<ValidatedScalarMultivariateFunctionModelDPInterface const>
        f1p=std::dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
    std::shared_ptr<ValidatedScalarMultivariateFunctionModelDPInterface const>
        f2p=std::dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
    if(f1p && f2p) {
        ValidatedScalarMultivariateFunctionModelDP f1m(f1p); ValidatedScalarMultivariateFunctionModelDP f2m(f2p); return join(f1m,f2m);
    } else if(f1p) {
        ValidatedScalarMultivariateFunctionModelDP f1m(f1p); ValidatedScalarMultivariateFunctionModelDP f2m=factory(f1m).create(f2); return join(f1m,f2m);
    } else if(f2p) {
        ValidatedScalarMultivariateFunctionModelDP f2m(f2p); ValidatedScalarMultivariateFunctionModelDP f1m=factory(f2m).create(f1); return join(f1m,f2m);
    } else {
        VectorOfScalarFunction<ValidatedTag,BoxDomainType> r(2u,f1.domain());
        r[0u]=f1;
        r[1u]=f2;
        return r;
    }
}


UpperIntervalType evaluate_range(ScalarMultivariateFunction<ValidatedTag>const& f, const Vector<UpperIntervalType>& x) {
    return static_cast<UpperIntervalType>(f(reinterpret_cast<Vector<ValidatedNumericType>const&>(x))); }
Vector<UpperIntervalType> evaluate_range(VectorMultivariateFunction<ValidatedTag>const& f, const Vector<UpperIntervalType>& x) {
    return static_cast<Vector<UpperIntervalType>>(f(reinterpret_cast<Vector<ValidatedNumericType>const&>(x))); }
Vector<Differential<UpperIntervalType>> derivative_range(VectorMultivariateFunction<ValidatedTag>const& f, const Vector<Differential<UpperIntervalType>>& x) {
    return static_cast<Vector<Differential<UpperIntervalType>>>(f(reinterpret_cast<Vector<Differential<ValidatedNumericType>>const&>(x))); }
Covector<UpperIntervalType> gradient_range(ValidatedScalarMultivariateFunction const& f, const Vector<UpperIntervalType>& x) {
    return static_cast<Covector<UpperIntervalType>>(static_cast<Covector<ValidatedNumericType>>(gradient(f,reinterpret_cast<Vector<ValidatedNumericType>const&>(x)))); }
Matrix<UpperIntervalType> jacobian_range(ValidatedVectorMultivariateFunction const& f, const Vector<UpperIntervalType>& x) {
    return static_cast<Matrix<UpperIntervalType>>(static_cast<Matrix<ValidatedNumericType>>(jacobian(f,reinterpret_cast<Vector<ValidatedNumericType>const&>(x)))); }

//------------------------ Function operators -------------------------------//

template<class P> VectorMultivariateFunction<P> join(const ScalarMultivariateFunction<P>& f1, const ScalarMultivariateFunction<P>& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    VectorMultivariateFunction<P> r(2,f1.domain());
    r.set(0,f1);
    r.set(1,f2);
    return r;
}

template<class P> VectorMultivariateFunction<P> join(const VectorMultivariateFunction<P>& f1, const ScalarMultivariateFunction<P>& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    VectorMultivariateFunction<P> r(f1.result_size()+1u,f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    r.set(f1.result_size(),f2);
    return r;
}

template<class P> VectorMultivariateFunction<P> join(const ScalarMultivariateFunction<P>& f1, const VectorMultivariateFunction<P>& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    VectorMultivariateFunction<P> r(f2.result_size()+1u,f1.domain());
    r.set(0u,f1);
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+1u,f2.get(i));
    }
    return r;
}

template<class P> VectorMultivariateFunction<P> join(const VectorMultivariateFunction<P>& f1, const VectorMultivariateFunction<P>& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    VectorMultivariateFunction<P> r(f1.result_size()+f2.result_size(),f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+f1.result_size(),f2.get(i));
    }
    return r;
}

ApproximateVectorMultivariateFunction join(const ApproximateScalarMultivariateFunction& f1, const ApproximateScalarMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    ApproximateVectorMultivariateFunction r(2,f1.domain());
    r.set(0,f1);
    r.set(1,f2);
    return r;
}

ApproximateVectorMultivariateFunction join(const ApproximateVectorMultivariateFunction& f1, const ApproximateScalarMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    ApproximateVectorMultivariateFunction r(f1.result_size()+1u,f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    r.set(f1.result_size(),f2);
    return r;
}

ApproximateVectorMultivariateFunction join(const ApproximateScalarMultivariateFunction& f1, const ApproximateVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    ApproximateVectorMultivariateFunction r(f2.result_size()+1u,f1.domain());
    r.set(0u,f1);
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+1u,f2.get(i));
    }
    return r;
}

ApproximateVectorMultivariateFunction join(const ApproximateVectorMultivariateFunction& f1, const ApproximateVectorMultivariateFunction& f2) {
    ARIADNE_ASSERT(f1.argument_size()==f2.argument_size());
    ApproximateVectorMultivariateFunction r(f1.result_size()+f2.result_size(),f1.domain());
    for(SizeType i=0u; i!=f1.result_size(); ++i) {
        r.set(i,f1.get(i));
    }
    for(SizeType i=0u; i!=f2.result_size(); ++i) {
        r.set(i+f1.result_size(),f2.get(i));
    }
    return r;
}


ApproximateScalarMultivariateFunction compose(const ApproximateScalarMultivariateFunction& f, const ApproximateVectorMultivariateFunction& g) {
    return make_composed_function(f,g);
}


ApproximateVectorMultivariateFunction compose(const ApproximateVectorMultivariateFunction& f, const ApproximateVectorMultivariateFunction& g) {
    return make_composed_function(f,g);
}


using std::dynamic_pointer_cast;

template<class A> struct AlgebraOperationsBase;

template<> struct AlgebraOperationsBase<ScalarMultivariateFunction<EffectiveTag>> {
    typedef EffectiveTag P;
    template<class OP> static EffectiveScalarMultivariateFunction apply(OP op, EffectiveScalarMultivariateFunction const& f) {
        auto e=dynamic_pointer_cast<const EffectiveScalarFormulaFunction>(f.managed_pointer());
        if(e) { return make_formula_function(e->_argument_size,op(e->_formula)); }
        else { return EffectiveScalarMultivariateFunction(new UnaryMultivaluedFunction<EffectiveTag>(op.code(),f)); }
    }
    template<class OP> static EffectiveScalarMultivariateFunction apply(OP op, EffectiveScalarMultivariateFunction const& f1, EffectiveScalarMultivariateFunction const& f2) {
        auto e1=dynamic_pointer_cast<const EffectiveScalarFormulaFunction>(f1.managed_pointer());
        auto e2=dynamic_pointer_cast<const EffectiveScalarFormulaFunction>(f2.managed_pointer());
        if(e1 && e2 && e1->_argument_size==e2->_argument_size) {
            return make_formula_function(e1->_argument_size,op(e1->_formula,e2->_formula));
        }
        else { return EffectiveScalarMultivariateFunction(new BinaryMultivaluedFunction<EffectiveTag>(op.code(),f1,f2)); }
    }
    template<class OP> static EffectiveScalarMultivariateFunction apply(OP op, EffectiveScalarMultivariateFunction const& f1, Number<P> const& c2) {
        auto e1=dynamic_pointer_cast<const EffectiveScalarFormulaFunction>(f1.managed_pointer());
        if(e1) { return make_formula_function(e1->_argument_size,op(e1->_formula,c2)); }
        else { return op(f1,EffectiveScalarMultivariateFunction::constant(f1.argument_size(),c2)); }
    }
    template<class OP> static EffectiveScalarMultivariateFunction apply(OP op, Number<P> const& c1, EffectiveScalarMultivariateFunction const& f2) {
        auto e2=dynamic_pointer_cast<const EffectiveScalarFormulaFunction>(f2.managed_pointer());
        if(e2) { return make_formula_function(e2->_argument_size,op(c1,e2->_formula)); }
        else { return op(EffectiveScalarMultivariateFunction::constant(f2.argument_size(),c1),f2); }
    }
    template<class OP> static EffectiveScalarMultivariateFunction apply(OP op, EffectiveScalarMultivariateFunction const& f, Int n) {
        return EffectiveScalarMultivariateFunction(new GradedMultivaluedFunction<P>(op.code(),f,n));
    }
};

template<> struct AlgebraOperationsBase<ScalarMultivariateFunction<ValidatedTag>> {
    template<class OP> static ValidatedScalarMultivariateFunction apply(OP op, ValidatedScalarMultivariateFunction const& f) {
        auto fp=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f.managed_pointer());
        if(fp) { ValidatedScalarMultivariateFunctionModelDP fm(fp); return op(fm); }
        else { return ValidatedScalarMultivariateFunction(new UnaryMultivaluedFunction<ValidatedTag>(op.code(),f)); }
    }

    template<class OP> static ValidatedScalarMultivariateFunction apply(OP op, ValidatedScalarMultivariateFunction const& f1, ValidatedScalarMultivariateFunction const& f2) {
        auto f1p=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
        auto f2p=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
        if(f1p && f2p) { ValidatedScalarMultivariateFunctionModelDP f1m(f1p); ValidatedScalarMultivariateFunctionModelDP f2m(f2p); return op(f1m,f2m); }
        else if(f1p) { ValidatedScalarMultivariateFunctionModelDP f1m(f1p); return op(f1m,factory(f1m).create(f2)); }
        else if(f2p) { ValidatedScalarMultivariateFunctionModelDP f2m(f2p); return op(factory(f2m).create(f1),f2m); }
        else { return ValidatedScalarMultivariateFunction(new BinaryMultivaluedFunction<ValidatedTag>(op.code(),f1,f2)); }
    }

    template<class OP> static ValidatedScalarMultivariateFunction apply(OP op, ValidatedScalarMultivariateFunction const& f1, ValidatedNumber const& c2) {
        auto f1p=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f1.managed_pointer());
        if(f1p) { ValidatedScalarMultivariateFunctionModelDP f1m=f1p; return op(f1,c2); }
        else { return ValidatedScalarMultivariateFunction(new BinaryMultivaluedFunction<ValidatedTag>(op.code(),f1,f1.create_constant(c2))); }
    }

    template<class OP> static ValidatedScalarMultivariateFunction apply(OP op, ValidatedNumber const& c1, ValidatedScalarMultivariateFunction const& f2) {
        auto f2p=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f2.managed_pointer());
        if(f2p) { ValidatedScalarMultivariateFunctionModelDP f2m=f2p; return op(c1,f2m); }
        else { return ValidatedScalarMultivariateFunction(new BinaryMultivaluedFunction<ValidatedTag>(op.code(),f2.create_constant(c1),f2)); }
    }

    static ValidatedScalarMultivariateFunction apply(Pow op, ValidatedScalarMultivariateFunction const& f, Int n) {
        auto fp=dynamic_pointer_cast<ValidatedScalarMultivariateFunctionModelDPInterface const>(f.managed_pointer());
        if(fp) { ValidatedScalarMultivariateFunctionModelDP fm=fp; return op(fm,n); }
        else { return ValidatedScalarMultivariateFunction(new GradedMultivaluedFunction<ValidatedTag>(op.code(),f,n)); }
    }
};

template<> struct AlgebraOperationsBase<ScalarMultivariateFunction<ApproximateTag>> {
    typedef ApproximateTag P;
    template<class OP> static ScalarMultivariateFunction<P> apply(OP op, ScalarMultivariateFunction<P> const& f) {
        return ScalarMultivariateFunction<P>(new UnaryMultivaluedFunction<P>(op.code(),f)); }
    template<class OP> static ScalarMultivariateFunction<P> apply(OP op, ScalarMultivariateFunction<P> const& f1, ScalarMultivariateFunction<P> const& f2) {
        return ScalarMultivariateFunction<P>(new BinaryMultivaluedFunction<P>(op.code(),f1,f2)); }
    template<class OP> static ScalarMultivariateFunction<P> apply(OP op, ScalarMultivariateFunction<P> const& f1, Number<P> const& c2) {
        return ScalarMultivariateFunction<P>(new BinaryMultivaluedFunction<P>(op.code(),f1,f1.create_constant(c2))); }
    template<class OP> static ScalarMultivariateFunction<P> apply(OP op, Number<P> const& c1, ScalarMultivariateFunction<P> const& f2) {
        return ScalarMultivariateFunction<P>(new BinaryMultivaluedFunction<P>(op.code(),f2.create_constant(c1),f2)); }
    template<class OP> static ScalarMultivariateFunction<P> apply(OP op, ScalarMultivariateFunction<P> const& f, Int n) {
        return ScalarMultivariateFunction<P>(new GradedMultivaluedFunction<P>(op.code(),f,n)); }
};

template<class P> struct AlgebraOperationsBase<ScalarUnivariateFunction<P>> {
    typedef IntervalDomainType D;
    template<class OP> static ScalarFunction<P,D> apply(OP op, ScalarFunction<P,D> const& f) {
        return ScalarFunction<P,D>(new UnaryFunction<P,D>(op.code(),f)); }
    template<class OP> static ScalarFunction<P,D> apply(OP op, ScalarFunction<P,D> const& f1, ScalarFunction<P,D> const& f2) {
        return ScalarFunction<P,D>(new BinaryFunction<P,D>(op.code(),f1,f2)); }
    template<class OP> static ScalarFunction<P,D> apply(OP op, ScalarFunction<P,D> const& f1, Number<P> const& c2) {
        return ScalarFunction<P,D>(new BinaryFunction<P,D>(op.code(),f1,f1.create_constant(c2))); }
    template<class OP> static ScalarFunction<P,D> apply(OP op, Number<P> const& c1, ScalarFunction<P,D> const& f2) {
        return ScalarFunction<P,D>(new BinaryFunction<P,D>(op.code(),f2.create_constant(c1),f2)); }
    template<class OP> static ScalarFunction<P,D> apply(OP op, ScalarFunction<P,D> const& f, Int n) {
        return ScalarFunction<P,D>(new GradedFunction<P,D>(op.code(),f,n)); }
};

template<class P, class D> using FunctionAlgebraOperations = AlgebraOperations<ScalarFunction<P,D>,Number<P>>;

template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Add op, F const& f1, F const& f2) -> F { return Base::apply(op,f1,f2); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Sub op, F const& f1, F const& f2) -> F { return Base::apply(op,f1,f2); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Mul op, F const& f1, F const& f2) -> F { return Base::apply(op,f1,f2); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Div op, F const& f1, F const& f2) -> F { return Base::apply(op,f1,f2); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Add op, F const& f, C const& c) -> F { return Base::apply(op,f,c); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Mul op, F const& f, C const& c) -> F { return Base::apply(op,f,c); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Pow op, F const& f, Int n) -> F { return Base::apply(op,f,n); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Pos op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Neg op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Sqr op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Rec op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Sqrt op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Exp op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Log op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Sin op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Cos op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Tan op, F const& f) -> F { return Base::apply(op,f); }
template<class P, class D> auto FunctionAlgebraOperations<P,D>::apply(Atan op, F const& f) -> F { return Base::apply(op,f); }

template struct AlgebraOperations<ScalarMultivariateFunction<ApproximateTag>,Number<ApproximateTag>>;
template struct AlgebraOperations<ScalarMultivariateFunction<ValidatedTag>,Number<ValidatedTag>>;
template struct AlgebraOperations<ScalarMultivariateFunction<EffectiveTag>,Number<EffectiveTag>>;

template struct AlgebraOperations<ScalarUnivariateFunction<ApproximateTag>,Number<ApproximateTag>>;
template struct AlgebraOperations<ScalarUnivariateFunction<ValidatedTag>,Number<ValidatedTag>>;
template struct AlgebraOperations<ScalarUnivariateFunction<EffectiveTag>,Number<EffectiveTag>>;


} // namespace Ariadne