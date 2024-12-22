#include "BSModel.hpp"



BSModel::BSModel(MathParams* params, RandomGeneration* generator)
    : r_(params->rate()),       
      fdStep_(params->fdStep()),
      SampleNb_(params->samples()),
      type_(params->type()),
      generator_(generator) {
    size_ = params->volatility()->m;
    volChol_ = pnl_mat_copy(params->volatility());
    paymentDates_ = pnl_vect_copy(params->dates());
}

BSModel::~BSModel() {
    pnl_mat_free(&volChol_);
    pnl_vect_free(&paymentDates_);
    
}

void BSModel::asset_path_at_0(PnlMat* path, const PnlVect* spots) {
    PnlMat* path_with_0 = pnl_mat_create(paymentDates_->size+1, size_);
    pnl_mat_set_row(path_with_0, spots, 0);
    
    for(int m = 0; m < paymentDates_->size; m++) {
        double t_prev = (m==0) ? 0: GET(paymentDates_, m-1);
        double t_next = GET(paymentDates_, m);
        double dt = t_next - t_prev;
        
        if(size_ == 1) {
            PnlVect *G = pnl_vect_create(size_);
            generator_->get_one_gaussian_sample(G);
            simulateOneStep1D(path_with_0, m+1, dt,G);
            pnl_vect_free(&G);
        } else {
            PnlVect *G = pnl_vect_create(size_);
            generator_->get_one_gaussian_sample(G);
            simulateOneStepND(path_with_0, m+1, dt,G);
            pnl_vect_free(&G);
        }
    }
    pnl_mat_extract_subblock(path, path_with_0, 1, path_with_0->m - 1, 0, path_with_0->n);
}

void BSModel::simulateOneStep1D(PnlMat* path, int currentIdx, double dt,PnlVect* g) const {
    
    
    double St = MGET(path, currentIdx-1, 0);
    double sigma = MGET(volChol_, 0, 0);
    double drift = (r_ - 0.5 * sigma * sigma) * dt;
    double diffusion = sigma * sqrt(dt) * GET(g, 0);
    MLET(path, currentIdx, 0) = St * exp(drift + diffusion);
    
   
}

void BSModel::simulateOneStepND(PnlMat* path, int currentIdx, double dt,PnlVect* G) const {
    double sqrtDt = sqrt(dt);
    PnlVect *correlated_gaussian = pnl_vect_create(size_);
    pnl_mat_mult_vect_inplace(correlated_gaussian, volChol_, G);
    
    for(int d = 0; d < size_; d++) {
        double St = MGET(path, currentIdx-1, d);
        
        
        PnlVect* L_d= pnl_vect_create(size_); 
        pnl_mat_get_row(L_d, volChol_,d);
        
        double drift = (r_ - 0.5 * pnl_vect_norm_two(L_d) * pnl_vect_norm_two(L_d)) * dt;
        double diffusion = sqrtDt  *  GET(correlated_gaussian, d);
        
        MLET(path, currentIdx, d) = St * exp(drift + diffusion);
    }
    pnl_vect_free(&correlated_gaussian);
    
}

void BSModel::asset_path(PnlMat* path, const PnlMat* past, double t,bool isMonitoringDate) {
    // Trouver l'index de la date de paiement <= t
    int end_copy_index;
    
    end_copy_index = (isMonitoringDate) ? past->m : past->m - 1;

    
    for (int j = 0; j<= end_copy_index; j++){
        PnlVect* temp_row=pnl_vect_create(size_);
        pnl_mat_get_row(temp_row,past,j);
        pnl_mat_set_row(path,temp_row,j);
        if (j == end_copy_index - 1 && isMonitoringDate) {
            PnlVect *temp_row = pnl_vect_create(size_);
            pnl_mat_get_row(temp_row, past, end_copy_index);
            pnl_mat_set_row(path, temp_row, end_copy_index);
            break;
        }
    }
    
    double tmp_date = t;
    // Simulation du reste de la trajectoire
    for (int i = end_copy_index; i < paymentDates_->size; i++) {
        double dt = GET(paymentDates_, i) - tmp_date;
        
        if (type_ == "VanillaCall") {
            PnlVect* g = pnl_vect_create(1);
            generator_->get_one_gaussian_sample(g);
            
            double St = MGET(past, past->m-1, 0);
            double sigma = MGET(volChol_, 0, 0);
            
            double drift = (r_ - 0.5 * sigma * sigma) * dt;
            double diffusion = sigma * sqrt(dt) * GET(g, 0);
            
            MLET(path, i, 0) = St * exp(drift + diffusion);
            
            pnl_vect_free(&g);
        } else {
            PnlVect *G = pnl_vect_create(size_);
            generator_->get_one_gaussian_sample(G);
            PnlVect *correlated_gaussian = pnl_vect_create(size_);
            pnl_mat_mult_vect_inplace(correlated_gaussian, volChol_, G);
            for(int d = 0; d < size_; d++) {
                double St;
                if (i == end_copy_index && !isMonitoringDate)
                    St = MGET(path, i, d);
                else
                    St = MGET(path, i - 1, d);
               
                PnlVect* L_d= pnl_vect_create(size_); 
                pnl_mat_get_row(L_d, volChol_,d);
                double vol = pnl_vect_norm_two(L_d);
                double drift = (r_ - 0.5 * vol * vol) * dt;
                double diffusion = sqrt(dt)  *  GET(correlated_gaussian, d);
                
                MLET(path, i, d) = St * exp(drift + diffusion);
            }
            pnl_vect_free(&correlated_gaussian);
            pnl_vect_free(&G);
        }
        tmp_date = GET(paymentDates_, i);
    }
    // std::cout<<"bs path done"<<std::endl;
}



void BSModel::shift_asset_path(PnlMat* shiftedPath, const PnlMat* path, int d, bool up,int from_idx) {
    // Copie intégrale du chemin original
    pnl_mat_clone(shiftedPath, path);
    // Shift de la dimension d à partir de l'index calculé
    for (int i = from_idx; i < path->m; i++) {
        if (up){
            double shift_val = MGET(shiftedPath, i, d) * (1.0 + fdStep_);
            MLET(shiftedPath, i, d) = shift_val;
        }else{
            double shift_val = MGET(shiftedPath, i, d) * (1.0 - fdStep_);
            MLET(shiftedPath, i, d) = shift_val;
        }
        
    }
}