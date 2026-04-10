import { Link } from 'react-router-dom';

const Guidelines = () => {
  return (
    <div className="px-6 py-12">
      <section className="site-card w-full p-8">
        <h2 className="text-3xl font-semibold mb-4 text-zinc-900 dark:text-slate-50 border-b-2 border-indigo-600/70 dark:border-indigo-400/40 pb-3">Guidelines to Authors</h2>
        <p className="text-lg text-zinc-700 dark:text-slate-200 mb-6">
            All papers must be submitted electronically through <span className='hover:cursor-pointer underline font-semibold text-indigo-700 dark:text-indigo-200' onClick={()=>window.open("https://cmt3.research.microsoft.com/ICNARI2027","_blank")}>Microsoft CMT</span>.
           We invite researchers, academicians, and industry professionals to submit papers presenting original, innovative, and high-quality research in line with the conference tracks. Submissions should reflect substantial contributions, including novel methodologies, experimental findings, case studies, or groundbreaking theoretical advancements.
        </p>

        <h3 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mt-6">Submission Guidelines:</h3>
        <ul className="list-disc pl-6 text-zinc-700 dark:text-slate-200 space-y-2 text-base">
          <li>The manuscripts must strictly adhere to the Springer template and page limits as follows: Regular Paper: 5 - 7 pages, and must concisely present the research problem, methodology, key findings, and conclusions.</li>
          <li>All submissions must align with the conference tracks and should demonstrate technical rigor and clarity.</li>
          <li>Manuscripts should be formatted as per the prescribed submission template and submitted via the submission link only.</li>
          <li>The manuscript must include sufficient technical details to facilitate a thorough review while maintaining a clear and concise narrative.</li>
        </ul>

        <h3 className="text-2xl font-semibold text-zinc-900 dark:text-slate-50 mt-6">Evaluation & Selection:</h3>
        <ul className="list-disc pl-6 text-zinc-700 dark:text-slate-200 space-y-2 text-base">
          <li>All submissions will undergo a peer-review process to ensure technical excellence, originality, and contribution to the field.</li>
          <li>Selected papers will be invited for presentation at the conference, with an opportunity for full paper submission in reputed journals or conference proceedings.</li>
        </ul>

        <p className="text-lg text-zinc-700 dark:text-slate-200 mt-6">Please refer to the author templates given below:</p>
        <ul className="list-disc pl-6 text-zinc-700 dark:text-slate-200 space-y-2 text-base">
          <li><strong>MS Word template:</strong> <Link target='blank' to="https://drive.google.com/drive/folders/1PwjhJU9x_VdgRv8rZyTBSAB4YEDuJtiF?usp=drive_link" className="text-indigo-700 dark:text-indigo-200 underline hover:text-indigo-600 dark:hover:text-indigo-100">Doc Template</Link></li>
          <li><strong>LaTex Formatting Macros:</strong> <Link target='blank' to="https://drive.google.com/drive/folders/1nzoBNldcT1HoLlvvkx5qTBtRIr7SXkTT?usp=drive_link" className="text-indigo-700 dark:text-indigo-200 underline hover:text-indigo-600 dark:hover:text-indigo-100">Tex Template</Link></li>
        </ul>
      </section>
    </div>
  );
}

export default Guidelines;
